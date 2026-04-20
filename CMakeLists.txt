cmake_minimum_required(VERSION 3.16)
project(EchoNote)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_ROOT "C:/Users/edward/AppData/Local/Programs/Qt/6.8.3/mingw_64") # desktop
# set(QT_ROOT "C:/ProgramData/Qt/6.8.3/mingw_64")                         # laptop

# Keep vcpkg-injected search paths; append Qt path instead of overwriting CMAKE_PREFIX_PATH.
list(APPEND CMAKE_PREFIX_PATH "${QT_ROOT}")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Multimedia
        MultimediaWidgets
        Svg
        REQUIRED)

find_package(taglib CONFIG REQUIRED)

add_executable(EchoNote src/cpp/main.cpp
        src/h/mainwindow.h
        src/cpp/mainwindow.cpp
        src/cpp/navigationbar.cpp
        src/h/navigationbar.h
        src/cpp/playercontrolbar.cpp
        src/h/playercontrolbar.h
        resources.qrc
        src/cpp/contentarea.cpp
        src/h/contentarea.h
        src/cpp/songpage.cpp
        src/h/songpage.h
        src/cpp/albumpage.cpp
        src/h/albumpage.h
        src/cpp/settingpage.cpp
        src/h/settingpage.h
        src/cpp/artistpage.cpp
        src/h/artistpage.h
        src/cpp/playlistpage.cpp
        src/h/playlistpage.h
        src/h/commonstruct.h
        src/cpp/scanmusic.cpp
        src/h/scanmusic.h
)

target_link_libraries(EchoNote
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Multimedia
        Qt::MultimediaWidgets
        Qt::Svg
        TagLib::tag
)

#target_link_libraries(EchoNote PRIVATE TagLib::tag TagLib::tag_c TagLib::TagLib)


if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${QT_ROOT}")
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

    if (EXISTS "${QT_INSTALL_PATH}/plugins/iconengines/qsvgicon${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/iconengines/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/iconengines/qsvgicon${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/iconengines/")
    endif ()

    foreach (QT_LIB Core Gui Widgets Multimedia MultimediaWidgets Svg)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)

    if (WIN32 AND MINGW)
        get_filename_component(MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
        set(MINGW_RUNTIME_DLLS
                "${MINGW_BIN_DIR}/libstdc++-6.dll"
                "${MINGW_BIN_DIR}/libwinpthread-1.dll"
                "${MINGW_BIN_DIR}/libgcc_s_seh-1.dll"
        )
        foreach (DLL ${MINGW_RUNTIME_DLLS})
            if (EXISTS ${DLL})
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy
                        "${DLL}"
                        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
                )
            endif ()
        endforeach ()
    endif ()

endif ()
