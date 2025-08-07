# CMakeLists.txt for SerialResponder project
cmake_minimum_required(VERSION 3.16)

set(PROJECT_SOURCES
    ./configuration/configmanager.h 
    ./configuration/configmanager.cpp
    ./custom_widgets/signalindicator.h
    ./custom_widgets/signalindicator.cpp
    ./responder/asciihexdelegate.h
    ./responder/asciihexdelegate.cpp
    ./responder/datatablemodel.h
    ./responder/datatablemodel.cpp
    ./responder/matcher.h
    ./responder/matcher.cpp
    ./responder/responderpackethandler.h
    ./responder/responderpackethandler.cpp
    ./responder/responderwidget.h 
    ./responder/responderwidget.cpp 
    ./responder/responderwidget.ui
    ./serialport/serialmanager.h
    ./serialport/serialmanager.cpp
    ./serialport/serialwidget.h
    ./serialport/serialwidget.cpp
    ./terminal/rowlinedelegate.h
    ./terminal/rowlinedelegate.cpp
    ./terminal/terminalsplitwidget.h
    ./terminal/terminalsplitwidget.cpp
    ./terminal/terminaltablemodel.h
    ./terminal/terminaltablemodel.cpp
    ./terminal/terminalviewwidget.h
    ./terminal/terminalviewwidget.cpp
    ./terminal/terminalinputwidget.h
    ./terminal/terminalinputwidget.cpp
    loggerwidget.h
    loggerwidget.cpp
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui   
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET SerialResponder APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME} ${PROJECT_SOURCES}
            )
    endif()
endif()

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ./configuration
    ./custom_widgets
    ./responder
    ./serialport
    ./terminal
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::SerialPort
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${PROJECT_NAME})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()
