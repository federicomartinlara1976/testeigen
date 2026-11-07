#include "operations.hpp"

#include <QStringBuilder>
#include <QTimer>

#include <QMatrix4x4>     // QtGui
#include <QQuaternion>    // QtGui
#include <QVector3D>     // QtGui

#include "eigen.hpp"

using namespace Qt::Literals::StringLiterals;

Operations::Operations(QObject *parent)
    : QObject(parent)
{
}

QString Operations::runBasics() {
    QString out;

    // Aritmética de matrices de tamaño fijo (conocido en compilación)
    Eigen::Matrix2d A;
    A << 1.0, 2.0,
         3.0, 4.0;

    Q_EMIT diagnosticReady(QStringLiteral("Básicos · A: %1")
                             .arg(EigenManager::diagnose(A)));

    Eigen::Matrix2d B = Eigen::Matrix2d::Identity();

    out += TestEigenHelper::section(QStringLiteral("A"), EigenManager::matrixToString(A));
    out += TestEigenHelper::section(QStringLiteral("B"), EigenManager::matrixToString(B));

    out += TestEigenHelper::section(QStringLiteral("A + B"), EigenManager::matrixToString(A + B));

    // Producto, transpuesta, inversa, determinante
    out += TestEigenHelper::section(QStringLiteral("A * A"), EigenManager::matrixToString(A * A));
    out += TestEigenHelper::section(QStringLiteral("A^T"), EigenManager::matrixToString(A.transpose()));
    out += TestEigenHelper::section(QStringLiteral("A^{-1}"), EigenManager::matrixToString(A.inverse()));
    out += QString(u"det(A) = %1\n"_s).arg(A.determinant());

    // Vectores dinámicos: norma, producto punto, cross (solo 3D)
    Eigen::VectorXd v(3);
    v << 1.0, 2.0, 3.0;
    out += QString(u"‖v‖ = %1, v·v = %2\n"_s)
               .arg(v.norm())
               .arg(v.dot(v));

    // Bloques y broadcasting (novedad práctica de la serie 3.x)
    Eigen::MatrixXd M = Eigen::MatrixXd::Constant(3, 4, 0.0);
    M.block<2, 2>(0, 0) = Eigen::Matrix2d::Ones(); // bloque superior-izq
    M.col(3) = v;                                   // asignar columna
    M.rowwise() -= M.colwise().mean();              // centrar filas
    out += TestEigenHelper::section(QStringLiteral("M (bloques/broadcast)"), EigenManager::matrixToString(M));

    return out;
}

QString Operations::runLinearSystems() {
    QString out;

    Eigen::Matrix3d A;
    A << 4.0, 1.0, 2.0,
         1.0, 5.0, 3.0,
         2.0, 3.0, 6.0;
    Eigen::Vector3d b(1.0, 0.0, -2.0);

    // el diagnóstico se emite mientras A existe
    Q_EMIT diagnosticReady(QStringLiteral("Sistemas · A: %1")
                             .arg(EigenManager::diagnose(A)));

    out += TestEigenHelper::section(QStringLiteral("A"), EigenManager::matrixToString(A));
    out += TestEigenHelper::section(QStringLiteral("b"), EigenManager::matrixToString(b));

    // Resolución directa (LU con pivoteo parcial, RCOND estimado)
    Eigen::Vector3d x1 = A.lu().solve(b);
    out += TestEigenHelper::section(QStringLiteral("A.lu().solve(b)"), EigenManager::matrixToString(x1));

    // Descomposición explícita: reutilizable para muchos lado-derechos
    Eigen::PartialPivLU<Eigen::Matrix3d> lu(A);
    Eigen::Vector3d x2 = lu.solve(b);
    out += TestEigenHelper::section(QStringLiteral("PartialPivLU reutilizable"), EigenManager::matrixToString(x2));

    // Verificación de residuo
    out += QString(u"‖A·x − b‖ = %1\n"_s).arg((A * x1 - b).norm());

    return out;
}

QString Operations::runEigenSolver() {
    QString out;

    Eigen::Matrix3d A;
    A << 2.0, 0.0, 0.0,
         0.0, 3.0, 1.0,
         0.0, 1.0, 3.0;

    // Smoke test
    // A(0, 0) = std::numeric_limits<double>::quiet_NaN();

    Q_EMIT diagnosticReady(QStringLiteral("Autovalores · A: %1")
                             .arg(EigenManager::diagnose(A)));

    out += TestEigenHelper::section(QStringLiteral("A"), EigenManager::matrixToString(A));

    // 1. EigenSolver general (admite autovalores complejos)
    Eigen::EigenSolver<Eigen::Matrix3d> es(A);
    if (es.info() == Eigen::Success) {
        out += TestEigenHelper::section(QStringLiteral("Autovalores (reales)"),
                              EigenManager::matrixToString(es.eigenvalues().real()));
        out += TestEigenHelper::section(QStringLiteral("Autovectores (parte real)"),
                              EigenManager::matrixToString(es.eigenvectors().real()));
    } else {
        Q_EMIT diagnosticReady(QStringLiteral("Autovalores · ⚠ EigenSolver falló"));
        out += QStringLiteral("EigenSolver: sin resultado (no convergió)\n");
    }

    // 2. Para matrices simétricas: SelfAdjointEigenSolver (más rápido y estable)
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> saes(A);
    if (saes.info() == Eigen::Success) {
        out += TestEigenHelper::section(QStringLiteral("SelfAdjointEigenSolver: valores"),
                              EigenManager::matrixToString(saes.eigenvalues()));
    } else {
        Q_EMIT diagnosticReady(QStringLiteral("Autovalores · ⚠ SelfAdjointEigenSolver falló"));
        out += QStringLiteral("SelfAdjointEigenSolver: sin resultado\n");
    }

    return out;
}

QString Operations::runSvd() {
    QString out;

    // Sistema sobredeterminado (4 ecuaciones, 2 incógnitas)
    Eigen::MatrixXd A(4, 2);
    A << 1.0, 0.0,
         0.0, 1.0,
         1.0, 1.0,
         1.0, -1.0;
    Eigen::Vector4d b(1.0, 2.0, 3.5, 0.5);

    Q_EMIT diagnosticReady(QStringLiteral("SVD · A: %1")
                             .arg(EigenManager::diagnose(A)));

    out += TestEigenHelper::section(QStringLiteral("A"), EigenManager::matrixToString(A));
    out += TestEigenHelper::section(QStringLiteral("b"), EigenManager::matrixToString(b));

    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
    out += TestEigenHelper::section(QStringLiteral("Valores singulares"),
                   EigenManager::matrixToString(svd.singularValues()));

    // Mínimos cuadrados: x minimiza ‖A·x − b‖
    Eigen::Vector2d x = svd.solve(b);
    out += TestEigenHelper::section(QStringLiteral("Solución mínimos cuadrados"), EigenManager::matrixToString(x));
    out += QString(u"Residuo ‖A·x − b‖ = %1\n"_s).arg((A * x - b).norm());

    return out;
}

QString Operations::runFactorizations(){
    QString out;

    Eigen::Matrix3d A;
    A << 4.0, 1.0, 0.0,
         1.0, 3.0, 1.0,
         0.0, 1.0, 2.0;

    Q_EMIT diagnosticReady(QStringLiteral("Factorizaciones · A: %1")
                             .arg(EigenManager::diagnose(A)));

    out += TestEigenHelper::section(QStringLiteral("A"), EigenManager::matrixToString(A));

    // Householder QR (para sistemas no cuadrados)
    Eigen::HouseholderQR<Eigen::Matrix3d> qr(A);
    out += TestEigenHelper::section(QStringLiteral("Q (HouseholderQR)"), EigenManager::matrixToString(Eigen::Matrix3d(qr.householderQ())));

    // Cholesky: válido solo si A es simétrica definida positiva
    Eigen::LLT<Eigen::Matrix3d> llt(A);
    if (llt.info() == Eigen::Success) {
        out += TestEigenHelper::section(QStringLiteral("L (Cholesky LLT)"), EigenManager::matrixToString(llt.matrixL().toDenseMatrix()));
    } else {
        Q_EMIT diagnosticReady(QStringLiteral("⚠ LLT: no definida positiva"));
    }

    return out;
}

QString Operations::runInteroperability() {
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
