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

TestEigenView::TestEigenView(QWidget *parent)
    : QWidget(parent)
{
    m_ui.setupUi(this);

    // conectar ANTES de ejecutar las operaciones
    connect(&m_context, &OperationContext::diagnosticReady, this, &TestEigenView::emitDiagnostics);

    // inicialización diferida: la ventana aparece primero
    QTimer::singleShot(0, this, &TestEigenView::loadResults);
}

void TestEigenView::loadResults()
{
    m_context.set_strategy(std::make_unique<BasicOperationStrategy>());
    m_ui.textEdit_1->setPlainText(m_context.doOperation());

    m_context.set_strategy(std::make_unique<LinearSystemsOperationStrategy>());
    m_ui.textEdit_2->setPlainText(m_context.doOperation());

    m_context.set_strategy(std::make_unique<EigenSolverOperationStrategy>());
    m_ui.textEdit_3->setPlainText(m_context.doOperation());

    m_context.set_strategy(std::make_unique<SvdOperationStrategy>());
    m_ui.textEdit_4->setPlainText(m_context.doOperation());

    m_context.set_strategy(std::make_unique<FactorizationOperationStrategy>());
    m_ui.textEdit_5->setPlainText(m_context.doOperation());

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
    const int size = TestEigenSettings::self()->matrixSize();

    m_ui.tableWidget_A->setRowCount(size);
    m_ui.tableWidget_A->setColumnCount(size);
    m_ui.tableWidget_b->setRowCount(size);
    m_ui.tableWidget_b->setColumnCount(1);

    // valores por defecto: identidad para A, unos para b
    const Eigen::MatrixXd A = Eigen::MatrixXd::Identity(size, size);
    const Eigen::VectorXd b = Eigen::VectorXd::Constant(size, 1.0);
    TestEigenHelper::matrixToTable(A, m_ui.tableWidget_A);
    TestEigenHelper::vectorToTable(b, m_ui.tableWidget_b);
}

// testeigenview.cpp
void TestEigenView::applySettings()
{
    initInteractiveTables();   // redimensiona tablas con el nuevo tamaño
}
