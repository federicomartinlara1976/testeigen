/*
    SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

// application headers
#include "testeigenview.h"
#include "testeigenhelper.hpp"

#include "TestEigenSettings.h"
#include "testeigendebug.h"

#include <QTimer>
#include <QStringBuilder>

using namespace Qt::Literals::StringLiterals;

TestEigenView::TestEigenView(QWidget *parent)
    : QWidget(parent)
{
    m_ui.setupUi(this);

    // conectar ANTES de ejecutar las operaciones
    connect(&m_context, &OperationContext::diagnosticReady, this, &TestEigenView::emitDiagnostics);

    connect(m_ui.btnCalcular, &QPushButton::clicked, this, &TestEigenView::calculateLinearSystems);
    connect(m_ui.btnCalcularSvd, &QPushButton::clicked, this, &TestEigenView::calculateSvd);
    connect(m_ui.btnCalcularEigenSolver, &QPushButton::clicked, this, &TestEigenView::calculateEigenSolver);
    connect(m_ui.btnCalcularFactorization, &QPushButton::clicked, this, &TestEigenView::calculateFactorization);

    // inicialización diferida: la ventana aparece primero
    QTimer::singleShot(0, this, &TestEigenView::loadInitialData);
}

void TestEigenView::loadInitialData()
{
    m_context.set_strategy(std::make_unique<BasicOperationStrategy>());
    m_ui.textEdit_1->setPlainText(m_context.doOperation());

    m_context.set_strategy(std::make_unique<InteroperabilityOperationStrategy>());
    m_ui.textEdit_6->setPlainText(m_context.doOperation());

    initInteractiveTables();
}

void TestEigenView::emitDiagnostics(const QString& message) {
    m_diagnostics << message;         // acumula siempre
    Q_EMIT diagnosticReady(accumulatedDiagnostics());   // reemite si alguien escucha
}

QString TestEigenView::accumulatedDiagnostics() const
{
    return m_diagnostics.join(u" · ");
}

void TestEigenView::initInteractiveTables()
{
    const auto [rows, cols] = TestEigenHelper::effectiveDimensions();

    m_ui.tableWidget_A->setRowCount(rows);
    m_ui.tableWidget_A->setColumnCount(cols);
    m_ui.tableWidget_b->setRowCount(rows);
    m_ui.tableWidget_b->setColumnCount(1);

    m_ui.tableWidget_svd_A->setRowCount(rows);
    m_ui.tableWidget_svd_A->setColumnCount(cols);
    m_ui.tableWidget_svd_b->setRowCount(rows);
    m_ui.tableWidget_svd_b->setColumnCount(1);

    // valores por defecto: identidad para A, unos para b
    const Eigen::MatrixXd A = Eigen::MatrixXd::Identity(rows, cols);
    const Eigen::VectorXd b = Eigen::VectorXd::Constant(rows, 1.0);

    TestEigenHelper::matrixToTable(A, m_ui.tableWidget_A);
    TestEigenHelper::matrixToTable(A, m_ui.tableWidget_svd_A);
    TestEigenHelper::matrixToTable(A, m_ui.tableWidget_eigensolver_A);
    TestEigenHelper::matrixToTable(A, m_ui.tableWidget_factorization_A);

    TestEigenHelper::vectorToTable(b, m_ui.tableWidget_b);
    TestEigenHelper::vectorToTable(b, m_ui.tableWidget_svd_b);
}

void TestEigenView::applySettings()
{
    initInteractiveTables();   // redimensiona tablas con el nuevo tamaño
}

void TestEigenView::calculateLinearSystems()
{
    QString error;
    OperationInput input;

    if (!TestEigenHelper::tableToMatrix(m_ui.tableWidget_A, input.A, &error)) {
        m_ui.textEdit_2->setPlainText(u"Error en A: %1\n"_s.arg(error));
        return;
    }
    if (!TestEigenHelper::tableToVector(m_ui.tableWidget_b, input.b, &error)) {
        m_ui.textEdit_2->setPlainText(u"Error en b: %1\n"_s.arg(error));
        return;
    }

    // el Context ya reenvía los diagnósticos a la barra de estado
    m_context.set_strategy(std::make_unique<LinearSystemsOperationStrategy>());
    m_ui.textEdit_2->setPlainText(m_context.doOperation(input));
}

void TestEigenView::calculateSvd()
{
    QString error;
    OperationInput input;

    if (!TestEigenHelper::tableToMatrix(m_ui.tableWidget_svd_A, input.A, &error)) {
        m_ui.textEdit_4->setPlainText(u"Error en A: %1\n"_s.arg(error));
        return;
    }
    if (!TestEigenHelper::tableToVector(m_ui.tableWidget_svd_b, input.b, &error)) {
        m_ui.textEdit_4->setPlainText(u"Error en b: %1\n"_s.arg(error));
        return;
    }

    // el Context ya reenvía los diagnósticos a la barra de estado
    m_context.set_strategy(std::make_unique<SvdOperationStrategy>());
    m_ui.textEdit_4->setPlainText(m_context.doOperation(input));
}

void TestEigenView::calculateEigenSolver()
{
    QString error;
    OperationInput input;

    if (!TestEigenHelper::tableToMatrix(m_ui.tableWidget_eigensolver_A, input.A, &error)) {
        m_ui.textEdit_3->setPlainText(u"Error en A: %1\n"_s.arg(error));
        return;
    }

    // el Context ya reenvía los diagnósticos a la barra de estado
    m_context.set_strategy(std::make_unique<EigenSolverOperationStrategy>());
    m_ui.textEdit_3->setPlainText(m_context.doOperation(input));
}

void TestEigenView::calculateFactorization()
{
    QString error;
    OperationInput input;

    if (!TestEigenHelper::tableToMatrix(m_ui.tableWidget_factorization_A, input.A, &error)) {
        m_ui.textEdit_5->setPlainText(u"Error en A: %1\n"_s.arg(error));
        return;
    }

    // el Context ya reenvía los diagnósticos a la barra de estado
    m_context.set_strategy(std::make_unique<FactorizationOperationStrategy>());
    m_ui.textEdit_5->setPlainText(m_context.doOperation(input));
}
