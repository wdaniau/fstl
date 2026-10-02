#### Fast .stl viewer #####
# Original Project Author: Matt Keeter Copyright 2014-2024
# Author: Paul Tsouchlos Copyright 2017-2024
# Author: William Daniau Copyright 2023-2025 (forked version fstl-e)

cmake_minimum_required(VERSION 3.16)

project(fstl-e)

# Setting -std=c++11
set(CMAKE_CXX_STANDARD 17)
# Setting standard to required, as requisted by DeveloperPaul123 on github
set(CXX_STANDARD_REQUIRED ON)

# Set the version number
# Renumerotation for fstl-e
set (FSTLE_VERSION_MAJOR "1")
set (FSTLE_VERSION_MINOR "3")
set (FSTLE_VERSION_PATCH "0-rc2")
set (PROJECT_VERSION "${FSTLE_VERSION_MAJOR}.${FSTLE_VERSION_MINOR}.${FSTLE_VERSION_PATCH}")

message(STATUS "Version: ${PROJECT_VERSION}")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)

#set project sources
set(Project_Sources src/app.cpp
src/backdrop.cpp
src/axis.cpp
src/canvas.cpp
src/glmesh.cpp
src/loader.cpp
src/main.cpp
src/mesh.cpp
src/window.cpp
src/shaderlightprefs.cpp
src/backdropsettingsdialog.cpp
src/speedmousedialog.cpp)

#set project headers.
set(Project_Headers src/app.h
src/backdrop.h
src/axis.h
src/canvas.h
src/glmesh.h
src/loader.h
src/mesh.h
src/window.h
src/shaderlightprefs.h
src/backdropsettingsdialog.h
src/speedmousedialog.h
)

#set project resources and icon resource
set(Project_Resources qt/qt.qrc gl/gl.qrc)
set(Icon_Resource exe/fstl-e.rc)

#set Policy CMP0072 FindOpenGL behavior
set(OpenGL_GL_PREFERENCE GLVND)

#find required packages.
#find_package(QT NAMES Qt6 Qt5 REQUIRED)

#find required packages.
# Qt Behavior
# Default is to look for Qt5 and Qt6
# If both are found Qt6 will be used
#
# If cmake is called with -DQT_DESIRED_VERSION=5 or 6
# it will look specifically for this version
set(QT_DESIRED_VERSION "" CACHE STRING "Force Qt version (5 or 6), auto choose if empty")
set_property(CACHE QT_DESIRED_VERSION PROPERTY STRINGS "" "5" "6")
if(QT_DESIRED_VERSION STREQUAL "5")
    find_package(QT NAMES Qt5 REQUIRED)
elseif(QT_DESIRED_VERSION STREQUAL "6")
    find_package(QT NAMES Qt6 REQUIRED)
elseif(QT_DESIRED_VERSION STREQUAL "")
    find_package(QT NAMES Qt6 Qt5 REQUIRED)
else()
    message(FATAL_ERROR "QT_DESIRED_VERSION invalid : '${QT_DESIRED_VERSION}' (valid values are '5', '6', or empty)")
endif()

set(QT_COMPONENTS Core Gui Widgets OpenGL)
set(QT_MIN_VERSION 5.12)
if(QT_VERSION_MAJOR EQUAL 6)
    list(APPEND QT_COMPONENTS OpenGLWidgets)
    set(QT_MIN_VERSION 6.2)
endif()
find_package(Qt${QT_VERSION_MAJOR} ${QT_MIN_VERSION} REQUIRED COMPONENTS ${QT_COMPONENTS})
message(STATUS "Using Qt: ${QT_VERSION_MAJOR}.${QT_VERSION_MINOR} (can override auto choice with -DQT_DESIRED_VERSION=5 or 6)")
find_package(Threads REQUIRED)

#add resources to RCC
qt_add_resources(Project_Resources_RCC ${Project_Resources})

#tell CMake AUTOGEN to skip autogen on the generated qrc files
set_property(SOURCE ${Project_Resources_RCC} PROPERTY SKIP_AUTOGEN ON)

if(WIN32)
  add_executable(fstl-e WIN32 ${Project_Sources} ${Project_Headers} ${Project_Resources_RCC} ${Icon_Resource})
  set(Fstle_LINK_FLAGS ${CMAKE_CURRENT_SOURCE_DIR}/${Icon_Resource})
  set_target_properties(fstl-e PROPERTIES LINK_FLAGS ${Fstle_LINK_FLAGS})
elseif(APPLE)
  add_executable(fstl-e MACOSX_BUNDLE ${Project_Sources} ${Project_Headers} ${Project_Resources_RCC} ${Icon_Resource})
else()
  add_executable(fstl-e ${Project_Sources} ${Project_Headers} ${Project_Resources_RCC} ${Icon_Resource})
endif(WIN32)

set(EXTRA_LIBS "")
if(QT_VERSION_MAJOR EQUAL 6)
    list(APPEND EXTRA_LIBS Qt6::OpenGLWidgets)
endif()
target_link_libraries(fstl-e
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::OpenGL
    ${EXTRA_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
)

# Add version definitions to use within the code.
target_compile_definitions(fstl-e PRIVATE -DFSTLE_VERSION="${PROJECT_VERSION}")

#installer information that is platform independent
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Fast .stl file viewer.")
set(CPACK_PACKAGE_VERSION_MAJOR ${FSTLE_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${FSTLE_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${FSTLE_VERSION_PATCH})

if(WIN32)

    set(QT_USE_QTMAIN true)

    if(MSVC)
        set_source_files_properties(fstl-e PROPERTIES LINKER_LANGUAGE "CXX")
        set_target_properties(fstl-e PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")

        install(TARGETS fstl-e DESTINATION bin COMPONENT all)

        # Base Qt DLLs, generic for Qt5/Qt6: the list adapts automatically
        # to the detected major version (QT_VERSION_MAJOR) and to the
        # OpenGLWidgets module, which only exists in Qt6.
        set(QT_RUNTIME_TARGETS
            Qt${QT_VERSION_MAJOR}::Core
            Qt${QT_VERSION_MAJOR}::Gui
            Qt${QT_VERSION_MAJOR}::OpenGL
            Qt${QT_VERSION_MAJOR}::Widgets
        )
        if(QT_VERSION_MAJOR EQUAL 6)
            list(APPEND QT_RUNTIME_TARGETS Qt6::OpenGLWidgets)
        endif()

        foreach(_qt_target ${QT_RUNTIME_TARGETS})
            install(FILES $<TARGET_FILE:${_qt_target}> DESTINATION bin COMPONENT all)
            add_custom_command(TARGET fstl-e POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_FILE:${_qt_target}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
            )
        endforeach()

        # EGL/GLESv2 DLLs: mandatory, since the app targets OpenGL ES 3.0
        # only. If this Qt build doesn't ship dynamic EGL/ANGLE support,
        # fail configuration early with a clear message instead of
        # producing a package that would crash at runtime.
        foreach(_gl_target Qt${QT_VERSION_MAJOR}::Gui_EGL Qt${QT_VERSION_MAJOR}::Gui_GLESv2)
            if(NOT TARGET ${_gl_target})
                message(FATAL_ERROR "${_gl_target} not found: this Qt build does not ship dynamic EGL/ANGLE support, required since this app targets OpenGL ES 3.0 only.")
            endif()
            install(FILES $<TARGET_FILE:${_gl_target}> DESTINATION bin COMPONENT all)
            add_custom_command(TARGET fstl-e POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    $<TARGET_FILE:${_gl_target}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
            )
        endforeach()

        #install file in the platforms directory. The path is resolved from
        #the Qt config directory that was actually found (Qt5 or Qt6).
        install (FILES
            ${Qt${QT_VERSION_MAJOR}Core_DIR}/../../../plugins/platforms/qwindows.dll
            DESTINATION bin/platforms COMPONENT all
        )
    endif(MSVC)

    # windows specific installer generation information
    set(CPACK_GENERATOR NSIS)
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL OFF)
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_NSIS_MUI_FINISHPAGE_RUN ${PROJECT_NAME})
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "${PROJECT_NAME}")
    set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\fstl-e.exe")
    set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/wdaniau/fstl")
    set(CPACK_NSIS_DISPLAY_NAME "fstl-e ${FSTLE_VERSION}")
    set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/exe/fstl-e.ico")
    set(CPACK_NSIS_MUI_UNIICON  "${CMAKE_CURRENT_SOURCE_DIR}/exe/fstl-e.ico")
    set(CPACK_NSIS_CREATE_ICONS_EXTRA
        "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\fstl-e.lnk' '$INSTDIR\\\\bin\\\\fstl-e.exe'")
    set(CPACK_COMPONENTS_ALL all)
    if (CMAKE_CL_64)
        set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
    else (CMAKE_CL_64)
        set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
    endif (CMAKE_CL_64)
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_FORMAT "UDBZ")
    set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}")
    set(CPACK_SYSTEM_NAME "OSX")
    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
    set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/app/fstl-e.icns")
else()
    # install target
    install(TARGETS fstl-e RUNTIME DESTINATION bin)
    # install desktop file
    install(FILES xdg/fstlapp-fstl-e.desktop DESTINATION share/applications)
    # install icons
    install(FILES xdg/icons/fstlapp-fstl-e_16x16.png DESTINATION share/icons/hicolor/16x16/apps RENAME fstlapp-fstl-e.png)
    install(FILES xdg/icons/fstlapp-fstl-e_22x22.png DESTINATION share/icons/hicolor/22x22/apps RENAME fstlapp-fstl-e.png)
    install(FILES xdg/icons/fstlapp-fstl-e_32x32.png DESTINATION share/icons/hicolor/32x32/apps RENAME fstlapp-fstl-e.png)
    install(FILES xdg/icons/fstlapp-fstl-e_48x48.png DESTINATION share/icons/hicolor/48x48/apps RENAME fstlapp-fstl-e.png)
    install(FILES xdg/icons/fstlapp-fstl-e_64x64.png DESTINATION share/icons/hicolor/64x64/apps RENAME fstlapp-fstl-e.png)
    install(FILES xdg/icons/fstlapp-fstl-e_128x128.png DESTINATION share/icons/hicolor/128x128/apps RENAME fstlapp-fstl-e.png)
    install(FILES xdg/icons/fstlapp-fstl-e_256x256.png DESTINATION share/icons/hicolor/256x256/apps RENAME fstlapp-fstl-e.png)
    # install metadata
    install(FILES metainfo/io.github.wdaniau.fstl.metainfo.xml DESTINATION share/metainfo)

    set(CPACK_PACKAGE_CONTACT "William Daniau")
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
    set(CPACK_GENERATOR "TGZ;DEB")
    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
endif(WIN32)

include(CPack)
