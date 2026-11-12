#include "operationstrategy.hpp"

#include <QString>
#include <QStringBuilder>

#include "testeigenhelper.hpp"
#include "eigen.hpp"

using namespace Qt::Literals::StringLiterals;

QString FactorizationOperationStrategy::doOperation(const OperationInput &input) {
    QString out;

    //Eigen::Matrix3d A;
    //A << 4.0, 1.0, 0.0,
    //     1.0, 3.0, 1.0,
    //     0.0, 1.0, 2.0;

    const auto &A = input.A;

    if (A.rows() != A.cols()) {
        Q_EMIT diagnosticReady(QStringLiteral("Factorizaciones · ⚠ A no es cuadrada"));
        return QStringLiteral("Error: las factorizaciones LU/Cholesky "
                              "requieren una matriz cuadrada.\n");
    }
    if (!A.allFinite()) {
        Q_EMIT diagnosticReady(QStringLiteral("Factorizaciones · ⚠ entrada con NaN/inf"));
        return QStringLiteral("Error: la entrada contiene NaN o infinitos.\n");
    }

    Q_EMIT diagnosticReady(QStringLiteral("Factorizaciones · A: %1")
                             .arg(EigenManager::diagnose(A)));

    out += TestEigenHelper::section(QStringLiteral("A"), EigenManager::matrixToString(A));

    // Householder QR (para sistemas no cuadrados)
    Eigen::HouseholderQR<Eigen::MatrixXd> qr(A);
    out += TestEigenHelper::section(QStringLiteral("Q (HouseholderQR)"), EigenManager::matrixToString(Eigen::MatrixXd(qr.householderQ())));

    // Cholesky: válido solo si A es simétrica definida positiva
    Eigen::LLT<Eigen::MatrixXd> llt(A);
    if (llt.info() == Eigen::Success) {
        out += TestEigenHelper::section(QStringLiteral("L (Cholesky LLT)"), EigenManager::matrixToString(llt.matrixL().toDenseMatrix()));
    } else {
        Q_EMIT diagnosticReady(QStringLiteral("⚠ LLT: no definida positiva"));
    }

    return out;
}
