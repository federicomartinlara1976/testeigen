#include "eigen.hpp"

// Diagnóstico numérico de una matriz: finitud, rango efectivo,
// determinante y condicionamiento (vía LU).
// Devuelve texto listo para una barra de estado.
QString EigenManager::diagnose(const Eigen::Ref<const Eigen::MatrixXd> &m)
{
    if (m.size() == 0) {
        return QStringLiteral("matriz vacía");
    }

    QStringList parts;

    // 1. NaN / infinitos
    if (!m.allFinite()) {
        parts << QStringLiteral("⚠ contiene NaN/inf");
    } else {
        parts << QStringLiteral("finita");
    }

    // 2. Cuadrada — ANTES de cualquier operación que exija filas == columnas
    if (m.rows() != m.cols()) {
        parts << QStringLiteral("no cuadrada (%1×%2)")
                     .arg(m.rows()).arg(m.cols());
        return parts.join(u" · ");   // adiós: nada de simetría/det/rcond
    }

    // 3. Simetría (ahora es seguro: es cuadrada)
    const bool symmetric = m.transpose().isApprox(m, 1e-10);
    parts << (symmetric ? QStringLiteral("simétrica")
                        : QStringLiteral("asimétrica"));

    // 4. Determinante
    const double det = m.determinant();
    parts << QStringLiteral("det: %1").arg(det, 0, 'g', 4);

    // 5. Condicionamiento vía LU
    if (m.allFinite()) {
        Eigen::PartialPivLU<Eigen::MatrixXd> lu(m);
        const double rcond = lu.rcond();
        if (rcond > 0.0 && rcond < 1e-12) {
            parts << QStringLiteral("⚠ singular (rcond %1)")
                         .arg(rcond, 0, 'g', 2);
        } else {
            parts << QStringLiteral("rcond: %1").arg(rcond, 0, 'g', 2);
        }
    }

    return parts.join(u" · ");
}
