/*
    SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

// application headers
#include "testeigenview.h"

#include "TestEigenSettings.h"
#include "testeigendebug.h"


TestEigenView::TestEigenView(QWidget *parent)
    : QWidget(parent)
{
    m_ui.setupUi(this);

    // conectar ANTES de ejecutar las operaciones
    connect(&operations, &Operations::diagnosticReady, this, &TestEigenView::emitDiagnostics);

    m_ui.textEdit_1->setPlainText(operations.runBasics());
    m_ui.textEdit_2->setPlainText(operations.runLinearSystems());
    m_ui.textEdit_3->setPlainText(operations.runEigenSolver());
    m_ui.textEdit_4->setPlainText(operations.runSvd());
    m_ui.textEdit_5->setPlainText(operations.runFactorizations());
}

void TestEigenView::emitDiagnostics(const QString& message) {
    m_diagnostics << message;         // acumula siempre
    Q_EMIT diagnosticReady(message);   // reemite si alguien escucha
}

QString TestEigenView::accumulatedDiagnostics() const
{
    return m_diagnostics.join(u" · ");
}

TestEigenView::~TestEigenView()
{
}

#include "moc_testeigenview.cpp"
