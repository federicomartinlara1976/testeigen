#ifndef __TESTEIGENHELPER_HPP__
#define __TESTEIGENHELPER_HPP__

#include <Eigen/Dense>
#include <QString>
#include <QTableWidget>

class TestEigenHelper {
public:
    static QString section(const QString &title, const QString &body);

    // nueva: QTableWidget → Eigen, con validación por celda
    static bool tableToMatrix(const QTableWidget *table,
                              Eigen::MatrixXd &out,
                              QString *error);

    static bool tableToVector(const QTableWidget *table,
                              Eigen::VectorXd &out,
                              QString *error);

    // tabla de una columna: para el vector b
    static void vectorToTable(const Eigen::VectorXd &v, QTableWidget *table);

    static void matrixToTable(const Eigen::MatrixXd &m, QTableWidget *table);
};

#endif
