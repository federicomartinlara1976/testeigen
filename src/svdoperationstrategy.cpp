#include "operationstrategy.hpp"

#include <QString>
#include <QStringBuilder>

#include "testeigenhelper.hpp"
#include "eigen.hpp"

using namespace Qt::Literals::StringLiterals;

QString SvdOperationStrategy::doOperation() {
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
