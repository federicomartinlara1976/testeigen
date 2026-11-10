#include "operationstrategy.hpp"

#include <QString>
#include <QStringBuilder>

#include "testeigenhelper.hpp"
#include "eigen.hpp"

using namespace Qt::Literals::StringLiterals;

QString EigenSolverOperationStrategy::doOperation(const OperationInput &input) {
    QString out;

    //Eigen::Matrix3d A;
    //A << 2.0, 0.0, 0.0,
    //     0.0, 3.0, 1.0,
    //     0.0, 1.0, 3.0;

    const auto &A = input.A;

    if (A.rows() != A.cols()) {
        Q_EMIT diagnosticReady(QStringLiteral("Autovalores · ⚠ A no es cuadrada"));
        return QStringLiteral("Error: los autovalores solo se definen "
                              "para matrices cuadradas.\n");
    }
    if (!A.allFinite()) {
        Q_EMIT diagnosticReady(QStringLiteral("Autovalores · ⚠ entrada con NaN/inf"));
        return QStringLiteral("Error: la entrada contiene NaN o infinitos.\n");
    }

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
