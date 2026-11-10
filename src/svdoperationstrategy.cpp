#include "operationstrategy.hpp"

#include <QString>
#include <QStringBuilder>

#include "testeigenhelper.hpp"
#include "eigen.hpp"

using namespace Qt::Literals::StringLiterals;

QString SvdOperationStrategy::doOperation(const OperationInput &input) {
    QString out;

    // Sistema sobredeterminado (4 ecuaciones, 2 incógnitas)
    //Eigen::MatrixXd A(4, 2);
    //A << 1.0, 0.0,
    //     0.0, 1.0,
    //     1.0, 1.0,
    //     1.0, -1.0;
    //Eigen::Vector4d b(1.0, 2.0, 3.5, 0.5);

    const auto &A = input.A;
    const auto &b = input.b;

    // Validación: SVD trabaja con sistemas sobredeterminados
    if (A.cols() > A.rows()) {
        Q_EMIT diagnosticReady(QStringLiteral("SVD · ⚠ A tiene más columnas que filas"));
        return QStringLiteral("Error: para mínimos cuadrados A debe tener "
                              "al menos tantas filas como columnas.\n");
    }
    if (A.rows() != b.size()) {
        Q_EMIT diagnosticReady(QStringLiteral("SVD · ⚠ b debe tener una fila por ecuación"));
        return QStringLiteral("Error: b debe tener %1 elementos (una fila por ecuación).\n")
                   .arg(A.rows());
    }
    if (!A.allFinite() || !b.allFinite()) {
        Q_EMIT diagnosticReady(QStringLiteral("SVD · ⚠ entrada con NaN/inf"));
        return QStringLiteral("Error: la entrada contiene NaN o infinitos.\n");
    }

    Q_EMIT diagnosticReady(QStringLiteral("SVD · A (%1×%2): %3")
                              .arg(A.rows()).arg(A.cols())
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
