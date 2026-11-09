#include "testeigenhelper.hpp"

#include <QStringBuilder>
#include <QTableWidget>
#include <QTableWidgetItem>

using namespace Qt::Literals::StringLiterals;

QString TestEigenHelper::section(const QString &title, const QString &body) {
    return u"== %1 ==\n%2\n"_s.arg(title, body);
}

// Helper de traducción: QTableWidget → Eigen (valida cada celda)
bool TestEigenHelper::tableToMatrix(const QTableWidget *table, Eigen::MatrixXd &out, QString *error)
{
    const int rows = table->rowCount();
    const int cols = table->columnCount();
    out.resize(rows, cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            bool ok = false;
            const double value = table->item(i, j) ? table->item(i, j)->text().toDouble(&ok)
                                                  : 0.0;
            if (!ok) {
                *error = QStringLiteral("celda (%1,%2) no numérica")
                             .arg(i + 1).arg(j + 1);
                return false;
            }
            out(i, j) = value;
        }
    }
    return true;
}

// tabla de una columna: para el vector b
void TestEigenHelper::vectorToTable(const Eigen::VectorXd &v, QTableWidget *table) {
    table->setRowCount(static_cast<int>(v.size()));
    table->setColumnCount(1);

    for (int i = 0; i < v.size(); ++i) {
        auto *item = new QTableWidgetItem(QString::number(v(i)));
        table->setItem(i, 0, item);   // QTableWidget se apropia: sin fugas ✅
    }
}

void TestEigenHelper::matrixToTable(const Eigen::MatrixXd& m, QTableWidget* table)
{
    table->setRowCount(static_cast<int>(m.rows()));
    table->setColumnCount(static_cast<int>(m.cols()));

    for (int i = 0; i < m.rows(); ++i) {
        for (int j = 0; j < m.cols(); ++j) {
            auto *item = new QTableWidgetItem(QString::number(m(i, j)));
            table->setItem(i, j, item);   // QTableWidget se apropia: sin fugas ✅
        }
    }
}

