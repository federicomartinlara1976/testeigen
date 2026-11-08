/*
    SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

// application headers
#include "testeigenview.h"

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
}

void TestEigenView::emitDiagnostics(const QString& message) {
    m_diagnostics << message;         // acumula siempre
    Q_EMIT diagnosticReady(accumulatedDiagnostics());   // reemite si alguien escucha
}

QString TestEigenView::accumulatedDiagnostics() const
{
    return m_diagnostics.join(u" · ");
}
