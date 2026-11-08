#include "operationstrategy.hpp"

#include <QString>
#include <QStringBuilder>

#include "testeigenhelper.hpp"
#include "eigen.hpp"

using namespace Qt::Literals::StringLiterals;

QString LinearSystemsOperationStrategy::doOperation() {
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
