#include "operationstrategy.hpp"

#include <QString>
#include <QStringBuilder>

#include "testeigenhelper.hpp"
#include "eigen.hpp"

using namespace Qt::Literals::StringLiterals;

QString BasicOperationStrategy::doOperation() {
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
