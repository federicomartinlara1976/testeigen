add_executable(testeigen)

target_sources(testeigen PRIVATE
    main.cpp
    testeigenwindow.cpp
    testeigenview.cpp
    testeigenhelper.cpp
    operations.cpp
    testeigen.qrc
)

ecm_qt_declare_logging_category(testeigen
    HEADER testeigendebug.h
    IDENTIFIER TESTEIGEN
    CATEGORY_NAME "testeigen"
)
kconfig_add_kcfg_files(testeigen TestEigenSettings.kcfgc)
ki18n_wrap_ui(testeigen
    testeigenview.ui
    settings.ui
)

target_link_libraries(testeigen
    KF6::CoreAddons
    KF6::I18n
    KF6::XmlGui
    KF6::ConfigWidgets
    KF6::DBusAddons
    KF6::Crash
    Qt6::Widgets
    Eigen3::Eigen
)

install(TARGETS testeigen ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.example.testeigen.desktop  DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.example.testeigen.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
