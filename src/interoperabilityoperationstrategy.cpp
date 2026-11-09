#include "operationstrategy.hpp"

#include <QString>
#include <QStringBuilder>

#include <QMatrix4x4>     // QtGui
#include <QQuaternion>    // QtGui
#include <QVector3D>     // QtGui

#include "testeigenhelper.hpp"
#include "eigen.hpp"

using namespace Qt::Literals::StringLiterals;

QString InteroperabilityOperationStrategy::doOperation(const OperationInput &input) {
    Q_UNUSED(input)

    QString out;

    // ---------- 1. QMatrix4x4 → Eigen::Matrix4f ----------
    // Map con RowMajor: interpreta los 16 floats internos de Qt
    // en su orden real. Sin copia: solo una vista.
    QMatrix4x4 qtRotation;
    qtRotation.rotate(45.0f, 0.0f, 0.0f, 1.0f);   // rotación de 45° en Z

    const Eigen::Map<const Eigen::Matrix4f, Eigen::RowMajor> view(qtRotation.constData());
    const Eigen::Matrix4f eRotation = view;       // copia materializada

    out += TestEigenHelper::section(QStringLiteral("QMatrix4x4 (rotación 45° Z)"),
                          EigenManager::matrixToString(eRotation));

    // ---------- 2. QVector3D → Eigen::Vector3f ----------
    const QVector3D qtPoint(1.0f, 2.0f, 3.0f);
    const Eigen::Vector3f ePoint(qtPoint.x(), qtPoint.y(), qtPoint.z());

    // Transformar el punto con Eigen:
    // homogeneous() añade la w=1 para multiplicar por la Matrix4f
    const Eigen::Vector3f eTransformed =
        (eRotation * ePoint.homogeneous()).hnormalized();

    // ---------- 3. Eigen::Matrix4f → QMatrix4x4 ----------
    // El constructor de QMatrix4x4 desde const float* espera fila-major:
    // usa almacenamiento RowMajor en Eigen para que los bytes coincidan.
    Eigen::Matrix<float, 4, 4, Eigen::RowMajor> eTranslation =
        Eigen::Matrix<float, 4, 4, Eigen::RowMajor>::Identity();
    eTranslation(0, 3) = 5.0f;   // traslación X
    eTranslation(1, 3) = -2.0f;  // traslación Y

    const QMatrix4x4 qtTranslation(eTranslation.data());

    // ---------- 4. Self-test: Qt y Eigen deben dar lo mismo ----------
    const QVector3D qtTransformedQt = qtRotation.map(qtPoint);   // vía Qt
    const QVector3D qtTransformedEigen(eTransformed.x(), eTransformed.y(),
                                       eTransformed.z());              // vía Eigen

    const bool agree = qFuzzyCompare(qtTransformedQt.distanceToPoint(qtTransformedEigen), 0.0f);

    out += TestEigenHelper::section(QStringLiteral("Punto transformado (Eigen)"),
                          EigenManager::matrixToString(eTransformed));
    out += QString(u"Verificación Qt↔Eigen: %1\n"_s)
               .arg(agree ? u"coinciden ✓"_s : u"⚠ NO coinciden"_s);

    Q_EMIT diagnosticReady(QStringLiteral("Interop · rotación 45°Z, %1")
                              .arg(agree ? QStringLiteral("Qt y Eigen de acuerdo")
                                         : QStringLiteral("⚠ discrepancia Qt/Eigen")));

    // ---------- 5. QQuaternion ↔ Eigen::Quaternionf ----------
    const QQuaternion qtQuat = QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, 90.0f);
    // constructor Eigen: (w, x, y, z); Qt: x,y,z + scalar aparte
    const Eigen::Quaternionf eQuat(qtQuat.scalar(),
                                   qtQuat.x(), qtQuat.y(), qtQuat.z());

    // Self-test: ambos quaternions deben rotar igual el mismo vector
    const QVector3D qtRotated = qtQuat.rotatedVector(QVector3D(1.0f, 0.0f, 0.0f));
    const Eigen::Vector3f eRotated = eQuat * Eigen::Vector3f(1.0f, 0.0f, 0.0f);

    const bool quatAgree =
        qFuzzyCompare(qtRotated.x(), eRotated.x())
        && qFuzzyCompare(qtRotated.y(), eRotated.y())
        && qFuzzyCompare(qtRotated.z(), eRotated.z());

    out += QString(u"Rotación 90°Y de (1,0,0): Qt(%1, %2, %3) vs Eigen(%4, %5, %6) → %7\n"_s)
               .arg(qtRotated.x()).arg(qtRotated.y()).arg(qtRotated.z())
               .arg(eRotated.x()).arg(eRotated.y()).arg(eRotated.z())
               .arg(quatAgree ? u"coinciden ✓"_s : u"⚠ NO coinciden"_s);

    return out;
}
