cmake_minimum_required(VERSION 3.16)
project(EchoPlayer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#set(QT_ROOT "C:/Users/edward/Programs/Qt/6.8.3/msvc2022_64")
#list(APPEND CMAKE_PREFIX_PATH "${QT_ROOT}")

# -----------------------------------------------------------------------------
# 跨平台 Qt 路径设置（仅在用户未通过命令行或环境变量指定时生效）
# -----------------------------------------------------------------------------
if (WIN32)
    # 将用户目录路径转换为 CMake 格式（正斜杠）
    file(TO_CMAKE_PATH "$ENV{USERPROFILE}/Programs/Qt/6.8.3/msvc2022_64" QT_DEFAULT_PATH)
elseif (UNIX AND NOT APPLE)
    file(TO_CMAKE_PATH "$ENV{HOME}/Programs/Qt/6.8.3/gcc_64" QT_DEFAULT_PATH)
endif ()

if (QT_DEFAULT_PATH AND EXISTS "${QT_DEFAULT_PATH}")
    list(APPEND CMAKE_PREFIX_PATH "${QT_DEFAULT_PATH}")
    message(STATUS "Added default Qt path: ${QT_DEFAULT_PATH}")
elseif (QT_DEFAULT_PATH AND NOT EXISTS "${QT_DEFAULT_PATH}")
    message(WARNING "Default Qt path '${QT_DEFAULT_PATH}' does not exist. "
            "Please set CMAKE_PREFIX_PATH or Qt6_DIR manually.")
endif ()

# 可选：打印最终的前缀路径，便于调试
message(STATUS "CMAKE_PREFIX_PATH = ${CMAKE_PREFIX_PATH}")


# -----------------------------------------------------------------------------
# 查找 Qt6 组件
# -----------------------------------------------------------------------------
find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Multimedia
        MultimediaWidgets
        Svg
        REQUIRED)

# -----------------------------------------------------------------------------
# 查找 taglib（vcpkg 安装）
# -----------------------------------------------------------------------------
find_package(taglib CONFIG REQUIRED)


# 获取 Qt6 安装根路径（用于后续 windeployqt 查找）
get_target_property(QT6_LOCATION Qt6::Core LOCATION)
if (QT6_LOCATION)
    get_filename_component(QT6_BIN_DIR "${QT6_LOCATION}" DIRECTORY)
    get_filename_component(QT_INSTALL_PREFIX "${QT6_BIN_DIR}" DIRECTORY)
endif ()

# vcpkg's taglib package may export TagLib::TagLib or TagLib::tag depending on version.
set(TAGLIB_LINK_TARGET "")
if (TARGET TagLib::TagLib)
    set(TAGLIB_LINK_TARGET TagLib::TagLib)
elseif (TARGET TagLib::tag)
    set(TAGLIB_LINK_TARGET TagLib::tag)
else ()
    message(FATAL_ERROR "taglib found, but no known CMake target exported (expected TagLib::TagLib or TagLib::tag)")
endif ()

# -----------------------------------------------------------------------------
# 添加可执行文件及源文件
# -----------------------------------------------------------------------------
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

# -----------------------------------------------------------------------------
# 链接库
# -----------------------------------------------------------------------------
target_link_libraries(EchoPlayer PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Qt::Multimedia
        Qt6::MultimediaWidgets
        Qt6::Svg
        ${TAGLIB_LINK_TARGET}
)

# -----------------------------------------------------------------------------
# Windows 部署（windeployqt）
# -----------------------------------------------------------------------------
if (WIN32)
    # Use Qt's deploy tool so vcpkg toolchain builds still copy all required runtime DLLs.
    if (NOT WINDEPLOYQT_EXECUTABLE)
        find_program(WINDEPLOYQT_EXECUTABLE
                NAMES windeployqt
                HINTS "${QT_ROOT}/bin")
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

endif ()

# -----------------------------------------------------------------------------
# Linux 部署（使用 Qt 官方部署工具）
# -----------------------------------------------------------------------------
if (UNIX AND NOT APPLE AND NOT WIN32)
    # 需要 CMake 3.18+ 才能使用 qt_generate_deploy_app_script
    if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.18)
        # 包含部署支持模块
        include(DeployQt6)

        # 生成部署脚本（自动处理 Qt 库和 taglib 等依赖）
        qt_generate_deploy_app_script(
                TARGET EchoPlayer
                OUTPUT_SCRIPT deploy_script
                DEPLOY_TOOL_OPTIONS --no-strip  # 可选：保留调试符号
        )

        # 在安装时执行部署脚本
        install(SCRIPT ${deploy_script})

        # 可选：在构建目录也生成一个可直接运行的部署脚本（方便测试）
        file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/deploy.sh CONTENT
                "#!/bin/bash\n${deploy_script}\necho 'Deployment complete. You can now run bin/EchoPlayer'")
        file(CHMOD ${CMAKE_CURRENT_BINARY_DIR}/deploy.sh PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE)

        message(STATUS "Linux deployment script generated: ${deploy_script}")
        message(STATUS "After installing (make install), the application will be self-contained.")
    else ()
        message(WARNING "CMake version >= 3.18 is required for automatic Linux deployment. "
                "You will need to manually run linuxdeployqt or ldd to collect dependencies.")
        # 降级方案：尝试查找 linuxdeployqt
        find_program(LINUXDEPLOYQT_EXECUTABLE linuxdeployqt)
        if (LINUXDEPLOYQT_EXECUTABLE)
            message(STATUS "Found linuxdeployqt, you can manually run: "
                    "${LINUXDEPLOYQT_EXECUTABLE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/EchoPlayer -bundle-non-qt-libs")
        endif ()
    endif ()
endif ()