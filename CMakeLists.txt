cmake_minimum_required(VERSION 3.23)
project(QR)

set(CMAKE_CXX_STANDARD 17)

ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/mingw81_64/lib/cmake")

set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/qr.rc")

find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

#OPEN XLSX
add_subdirectory(OpenXLSX)

add_executable(QR WIN32 main.cpp
    ${APP_ICON_RESOURCE_WINDOWS}
    XlsFormat.h
    BasicXls.h
    BasicXls.cpp
    XlsFormat.cpp

    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    xlsConverter.h
    xlsConverter.cpp

    qrDialog.h
    qrDialog.cpp
    qrDialog.ui

    txtfiles.h
    txtfiles.cpp
    txtfiles.ui

    containers.h
    containers.cpp
    containers.ui

    help.h
    help.cpp
    help.ui

    config.h
    config.cpp

    version.h
    version.cpp
)

target_link_libraries(QR
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        OpenXLSX::OpenXLSX
        )

#target_link_libraries(QR PRIVATE OpenXLSX::OpenXLSX)

    if (WIN32)
        set(DEBUG_SUFFIX)
        set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
            if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
                set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
            endif ()
        endif ()
        if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        endif ()
        foreach (QT_LIB Core Gui Widgets)
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    "${QT_INSTALL_PATH}/bin/Qt5${QT_LIB}${DEBUG_SUFFIX}.dll"
                    "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
        endforeach (QT_LIB)
    endif ()
