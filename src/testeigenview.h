/*
    SPDX-FileCopyrightText: %{CURRENT_YEAR} %{AUTHOR} <%{EMAIL}>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#ifndef TESTEIGENVIEW_H
#define TESTEIGENVIEW_H

#include "ui_testeigenview.h"


/**
 * This class serves as the main window for testeigen.  It handles the
 * menus, toolbars and status bars.
 *
 * @short Main window class
 * @author %{AUTHOR} <%{EMAIL}>
 * @version %{VERSION}
 */
class TestEigenView : public QWidget
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    explicit TestEigenView(QWidget *parent);

    /**
     * Default Destructor
     */
    ~TestEigenView() override;

public Q_SLOTS:
    void switchColors();
    void handleSettingsChanged();

private:
    // this is the name of the root widget inside our Ui file
    // you can rename it in designer and then change it here
    Ui::TestEigenView m_ui;
};

#endif // TESTEIGENVIEW_H
