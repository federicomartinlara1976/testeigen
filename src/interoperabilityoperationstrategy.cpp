#include "operationstrategy.hpp"

#include <QString>
#include <QStringBuilder>

#include <QMatrix4x4>     // QtGui
#include <QQuaternion>    // QtGui
#include <QVector3D>     // QtGui
#include <cmath>

#include "testeigenhelper.hpp"
#include "eigen.hpp"

using namespace Qt::Literals::StringLiterals;

QString InteroperabilityOperationStrategy::doOperation(const OperationInput &input) {
    QString out;

    // Validación: transformación 4×4 y punto 3×1
    if (input.transform.rows() != 4 || input.transform.cols() != 4) {
        Q_EMIT diagnosticReady(QStringLiteral("Interop · ⚠ la transformación debe ser 4×4"));
        return QStringLiteral("Error: la transformación debe ser una matriz 4×4.\n");
    }
    if (input.point.size() != 3) {
        Q_EMIT diagnosticReady(QStringLiteral("Interop · ⚠ el punto debe ser 3×1"));
        return QStringLiteral("Error: el punto debe tener 3 componentes.\n");
    }
    if (!input.transform.allFinite() || !input.point.allFinite()) {
        Q_EMIT diagnosticReady(QStringLiteral("Interop · ⚠ entrada con NaN/inf"));
        return QStringLiteral("Error: la entrada contiene NaN o infinitos.\n");
    }

    // ---------- 1. Input (double) → float: la decisión de precisión ----------
    // En gráficos la GPU es float: convertimos al final, como acordamos.
    const Eigen::Matrix4f eTransform = input.transform.cast<float>();
    const Eigen::Vector3f ePoint(input.point(0), input.point(1), input.point(2));

    // ---------- 2. Eigen → Qt: RowMajor para que los bytes coincidan ----------
    const Eigen::Matrix<float, 4, 4, Eigen::RowMajor> eRowMajor = eTransform;
    const QMatrix4x4 qtTransform(eRowMajor.data());   // constructor espera fila-major

    // ---------- 3. El mismo punto, transformado por ambos ----------
    const Eigen::Vector3f eTransformed =
        (eTransform * ePoint.homogeneous()).hnormalized();   // vía Eigen
    const QVector3D qtPoint(ePoint.x(), ePoint.y(), ePoint.z());
    const QVector3D qtTransformed = qtTransform.map(qtPoint); // vía Qt (map, no *)

    // ---------- 4. Self-test: comparar el efecto, no la representación ----------
    const bool agree =
        qFuzzyCompare(qtTransformed.x(), eTransformed.x())
        && qFuzzyCompare(qtTransformed.y(), eTransformed.y())
        && qFuzzyCompare(qtTransformed.z(), eTransformed.z());

    out += TestEigenHelper::section(QStringLiteral("Transformación (del usuario)"),
                                    EigenManager::matrixToString(input.transform));
    out += TestEigenHelper::section(QStringLiteral("Punto transformado (Eigen)"),
                                    EigenManager::matrixToString(eTransformed));
    out += QString(u"Punto transformado (Qt): (%1, %2, %3)\n"_s)
               .arg(qtTransformed.x()).arg(qtTransformed.y()).arg(qtTransformed.z());
    out += QString(u"Verificación Qt↔Eigen: %1\n"_s)
               .arg(agree ? u"coinciden ✓"_s : u"⚠ NO coinciden"_s);

    Q_EMIT diagnosticReady(QStringLiteral("Interop · punto (%1, %2, %3) → %4")
                              .arg(eTransformed.x()).arg(eTransformed.y())
                              .arg(eTransformed.z())
                              .arg(agree ? u"Qt y Eigen de acuerdo"_s
                                         : u"⚠ discrepancia Qt/Eigen"_s));

    // ---------- 5. El self-test del quaternion queda fijo (demo) ----------
    // (su input natural sería otro: eje + ángulo; lo dejamos como demo interna)
    const QQuaternion qtQuat = QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, 90.0f);
    const Eigen::Quaternionf eQuat(qtQuat.scalar(),
                                   qtQuat.x(), qtQuat.y(), qtQuat.z());
    const QVector3D qtRotated = qtQuat.rotatedVector(QVector3D(1.0f, 0.0f, 0.0f));
    const Eigen::Vector3f eRotated = eQuat * Eigen::Vector3f(1.0f, 0.0f, 0.0f);
    // Comparación absoluta: qFuzzyCompare (relativa) falla cerca de cero,
    // y rotar (1,0,0) 90° en Y produce componentes ≈ 0
    const auto closeEnough = [](float a, float b) {
        return std::abs(a - b) < 1e-5f;
    };
    const bool quatAgree = closeEnough(qtRotated.x(), eRotated.x())
                           && closeEnough(qtRotated.y(), eRotated.y())
                           && closeEnough(qtRotated.z(), eRotated.z());

    out += QString(u"Self-test quaternion (demo): %1\n"_s)
               .arg(quatAgree ? u"coinciden ✓"_s : u"⚠ NO coinciden"_s);

    return out;
}
