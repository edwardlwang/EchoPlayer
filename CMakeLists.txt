cmake_minimum_required(VERSION 3.16)
project(EchoPlayer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_ROOT "C:/Users/edward/Programs/Qt/6.8.3/msvc2022_64")

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


# vcpkg's taglib package may export TagLib::TagLib or TagLib::tag depending on version.
set(TAGLIB_LINK_TARGET "")
if (TARGET TagLib::TagLib)
    set(TAGLIB_LINK_TARGET TagLib::TagLib)
elseif (TARGET TagLib::tag)
    set(TAGLIB_LINK_TARGET TagLib::tag)
else ()
    message(FATAL_ERROR "taglib found, but no known CMake target exported (expected TagLib::TagLib or TagLib::tag)")
endif ()

add_executable(EchoPlayer src/cpp/main.cpp
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
        src/cpp/playbackcontroller.cpp
        src/h/playbackcontroller.h
)

target_link_libraries(EchoPlayer PRIVATE
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Multimedia
        Qt::MultimediaWidgets
        Qt::Svg
        TagLib::tag
)

#target_link_libraries(EchoPlayer PRIVATE TagLib::tag TagLib::tag_c TagLib::TagLib)


if (WIN32)
    # Use Qt's deploy tool so vcpkg toolchain builds still copy all required runtime DLLs.
    if (NOT WINDEPLOYQT_EXECUTABLE)
        find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS "${QT_ROOT}/bin")
    endif ()

    if (WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND "${WINDEPLOYQT_EXECUTABLE}"
                --no-translations
                --no-system-d3d-compiler
                --no-opengl-sw
                "$<$<CONFIG:Debug>:--debug>$<$<NOT:$<CONFIG:Debug>>:--release>"
                "$<TARGET_FILE:${PROJECT_NAME}>"
                COMMENT "Deploying Qt runtime next to ${PROJECT_NAME}.exe"
                VERBATIM)
    else ()
        message(WARNING "windeployqt not found; runtime launch may fail with missing Qt DLLs")
    endif ()

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
