#include "operations.hpp"

#include <QStringBuilder>

#include "eigen.hpp"

using namespace Qt::Literals::StringLiterals;

QString Operations::runBasics() {
    QString out;

    // Aritmética de matrices de tamaño fijo (conocido en compilación)
    Eigen::Matrix2d A;
    A << 1.0, 2.0,
         3.0, 4.0;
    Eigen::Matrix2d B = Eigen::Matrix2d::Identity();

    out += helper.section(QStringLiteral("A"), EigenManager::matrixToString(A));
    out += helper.section(QStringLiteral("B"), EigenManager::matrixToString(B));

    out += helper.section(QStringLiteral("A + B"), EigenManager::matrixToString(A + B));

    // Producto, transpuesta, inversa, determinante
    out += helper.section(QStringLiteral("A * A"), EigenManager::matrixToString(A * A));
    out += helper.section(QStringLiteral("A^T"), EigenManager::matrixToString(A.transpose()));
    out += helper.section(QStringLiteral("A^{-1}"), EigenManager::matrixToString(A.inverse()));
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
    out += helper.section(QStringLiteral("M (bloques/broadcast)"), EigenManager::matrixToString(M));

    return out;
}

QString Operations::runLinearSystems() {
    QString out;

    Eigen::Matrix3d A;
    A << 4.0, 1.0, 2.0,
         1.0, 5.0, 3.0,
         2.0, 3.0, 6.0;
    Eigen::Vector3d b(1.0, 0.0, -2.0);

    out += helper.section(QStringLiteral("A"), EigenManager::matrixToString(A));
    out += helper.section(QStringLiteral("b"), EigenManager::matrixToString(b));

    // Resolución directa (LU con pivoteo parcial, RCOND estimado)
    Eigen::Vector3d x1 = A.lu().solve(b);
    out += helper.section(QStringLiteral("A.lu().solve(b)"), EigenManager::matrixToString(x1));

    // Descomposición explícita: reutilizable para muchos lado-derechos
    Eigen::PartialPivLU<Eigen::Matrix3d> lu(A);
    Eigen::Vector3d x2 = lu.solve(b);
    out += helper.section(QStringLiteral("PartialPivLU reutilizable"), EigenManager::matrixToString(x2));

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

    out += helper.section(QStringLiteral("A"), EigenManager::matrixToString(A));

    Eigen::EigenSolver<Eigen::Matrix3d> es(A);
    out += helper.section(QStringLiteral("Autovalores (reales)"),
                   EigenManager::matrixToString(es.eigenvalues().real()));
    out += helper.section(QStringLiteral("Autovectores (parte real)"),
                   EigenManager::matrixToString(es.eigenvectors().real()));

    // Para matrices simétricas: SelfAdjointEigenSolver (más rápido y estable)
    Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> saes(A);
    out += helper.section(QStringLiteral("SelfAdjointEigenSolver: valores"),
                   EigenManager::matrixToString(saes.eigenvalues()));

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

    out += helper.section(QStringLiteral("A"), EigenManager::matrixToString(A));
    out += helper.section(QStringLiteral("b"), EigenManager::matrixToString(b));

    Eigen::JacobiSVD<Eigen::MatrixXd> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
    out += helper.section(QStringLiteral("Valores singulares"),
                   EigenManager::matrixToString(svd.singularValues()));

    // Mínimos cuadrados: x minimiza ‖A·x − b‖
    Eigen::Vector2d x = svd.solve(b);
    out += helper.section(QStringLiteral("Solución mínimos cuadrados"), EigenManager::matrixToString(x));
    out += QString(u"Residuo ‖A·x − b‖ = %1\n"_s).arg((A * x - b).norm());
    return out;

    return out;
}

QString Operations::runFactorizations(){
    QString out;

    Eigen::Matrix3d A;
    A << 4.0, 1.0, 0.0,
         1.0, 3.0, 1.0,
         0.0, 1.0, 2.0;

    out += helper.section(QStringLiteral("A"), EigenManager::matrixToString(A));

    // Householder QR (para sistemas no cuadrados)
    Eigen::HouseholderQR<Eigen::Matrix3d> qr(A);
    out += helper.section(QStringLiteral("Q (HouseholderQR)"), EigenManager::matrixToString(Eigen::Matrix3d(qr.householderQ())));

    // Cholesky: válido solo si A es simétrica definida positiva
    Eigen::LLT<Eigen::Matrix3d> llt(A);
    if (llt.info() == Eigen::Success) {
        out += helper.section(QStringLiteral("L (Cholesky LLT)"), EigenManager::matrixToString(llt.matrixL().toDenseMatrix()));
    } else {
        out += QStringLiteral("LLT: matriz no definida positiva\n");
    }

    return out;
}
