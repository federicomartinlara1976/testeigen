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
    handleSettingsChanged();
}

TestEigenView::~TestEigenView()
{
}

void TestEigenView::switchColors()
{
    // switch the foreground/background colors of the label
    QColor color = TestEigenSettings::colorBackground();
    TestEigenSettings::setColorBackground(TestEigenSettings::colorForeground());
    TestEigenSettings::setColorForeground(color);

    handleSettingsChanged();
}

void TestEigenView::handleSettingsChanged()
{
    qCDebug(TESTEIGEN) << "TestEigenView::handleSettingsChanged()";
    QPalette palette = m_ui.templateLabel->palette();
    palette.setColor(QPalette::Window, TestEigenSettings::colorBackground());
    palette.setColor(QPalette::WindowText, TestEigenSettings::colorForeground());
    m_ui.templateLabel->setPalette(palette);

    // i18n : internationalization
    m_ui.templateLabel->setText(i18n("This project is %1 days old", TestEigenSettings::ageInDays()));
}

#include "moc_testeigenview.cpp"
