/*
    SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#ifndef TESTEIGENWINDOW_H
#define TESTEIGENWINDOW_H

#include <KXmlGuiWindow>

#include "ui_settings.h"
#include "TestEigenSettings.h"

class TestEigenView;

/**
 * This class serves as the main window for testeigen.  It handles the
 * menus, toolbars and status bars.
 *
 * @short Main window class
 * @author %{AUTHOR} <%{EMAIL}>
 * @version %{VERSION}
 */
class TestEigenWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    TestEigenWindow();

    /**
     * Default Destructor
     */
    ~TestEigenWindow() = default;

private Q_SLOTS:
    /**
     * Open the settings dialog
     */
    void settingsConfigure();

    void showDiagnostics(const QString& message);

    void handleSettingsChanged();

private:
    // this is the name of the root widget inside our Ui file
    // you can rename it in designer and then change it here
    Ui::Settings m_settings;
    TestEigenView *m_testeigenView;
};

#endif // TESTEIGENWINDOW_H
