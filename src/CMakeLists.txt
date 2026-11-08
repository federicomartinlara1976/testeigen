add_executable(testeigen)

target_sources(testeigen PRIVATE
    main.cpp
    testeigenwindow.cpp
    testeigenview.cpp
    testeigenhelper.cpp
    eigen.cpp
    operationcontext.cpp
    operationstrategy.hpp # Si se definen clases con Q_OBJECT, hay que poner aquí las cabeceras para que compile el moc
    basicoperationstrategy.cpp
    linearsystemsoperationstrategy.cpp
    eigensolveroperationstrategy.cpp
    svdoperationstrategy.cpp
    factorizationoperationstrategy.cpp
    interoperabilityoperationstrategy.cpp
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

#target_compile_options(testeigen PRIVATE -fsanitize=address -fno-omit-frame-pointer)

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
install(PROGRAMS net.bounceme.chronos.testeigen.desktop  DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES net.bounceme.chronos.testeigen.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
