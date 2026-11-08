/*
    SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

// application headers
#include "testeigenwindow.h"

#include "testeigenview.h"
#include "testeigendebug.h"

// KF headers
#include <KActionCollection>
#include <KConfigDialog>

#include <QStatusBar>


TestEigenWindow::TestEigenWindow()
    : KXmlGuiWindow()
{
    m_testeigenView = new TestEigenView(this);
    setCentralWidget(m_testeigenView);

    KActionCollection* actionCollection = this->actionCollection();

    KStandardAction::quit(qApp, SLOT(closeAllWindows()), actionCollection);
    KStandardAction::preferences(this, SLOT(settingsConfigure()), actionCollection);

    setupGUI();

    setWindowTitle(QStringLiteral("Eigen 3.4 Playground — Qt/KDE"));

    connect(m_testeigenView, &TestEigenView::diagnosticReady, this, &TestEigenWindow::showDiagnostics);
    // los emitidos durante la construcción de la vista:
    statusBar()->showMessage(m_testeigenView->accumulatedDiagnostics());
}

void TestEigenWindow::showDiagnostics(const QString& message) {
    qCDebug(TESTEIGEN) << message;
    statusBar()->showMessage(message);
}

TestEigenWindow::~TestEigenWindow()
{
}

void TestEigenWindow::settingsConfigure()
{
    qCDebug(TESTEIGEN) << "TestEigenWindow::settingsConfigure()";
    // The preference dialog is derived from prefs_base.ui
    //
    // compare the names of the widgets in the .ui file
    // to the names of the variables in the .kcfg file
    //avoid to have 2 dialogs shown
    if (KConfigDialog::showDialog(QStringLiteral("settings"))) {
        return;
    }

    KConfigDialog *dialog = new KConfigDialog(this, QStringLiteral("settings"), TestEigenSettings::self());
    QWidget *generalSettingsPage = new QWidget;
    m_settings.setupUi(generalSettingsPage);
    dialog->addPage(generalSettingsPage, i18nc("@title:tab", "General"), QStringLiteral("package_setting"));
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}
