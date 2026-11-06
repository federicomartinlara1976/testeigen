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

    m_ui.textEdit_1->setPlainText(operations.runBasics());
    handleSettingsChanged();
}

TestEigenView::~TestEigenView()
{
}

void TestEigenView::handleSettingsChanged()
{
    qCDebug(TESTEIGEN) << "TestEigenView::handleSettingsChanged()";
}

#include "moc_testeigenview.cpp"
