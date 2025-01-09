#### Fast .stl viewer #####
# Original Project Author: Matt Keeter Copyright 2014-2024
# Author: Paul Tsouchlos Copyright 2017-2024
# Author: William Daniau Copyright 2023-2025 (forked version efstl)

cmake_minimum_required(VERSION 3.3)

project(fstl-e)

# Setting -std=c++11
set(CMAKE_CXX_STANDARD 11)
# Setting standard to required, as requisted by DeveloperPaul123 on github
set(CXX_STANDARD_REQUIRED ON)

# Set the version number
# Renumerotation for fstl-e
set (FSTLE_VERSION_MAJOR "1")
set (FSTLE_VERSION_MINOR "0")
set (FSTLE_VERSION_PATCH "0")
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
src/shaderlightprefs.cpp)

#set project headers. 
set(Project_Headers src/app.h
src/backdrop.h
src/axis.h
src/canvas.h
src/glmesh.h
src/loader.h
src/mesh.h
src/window.h
src/shaderlightprefs.h)

#set project resources and icon resource
set(Project_Resources qt/qt.qrc gl/gl.qrc)
set(Icon_Resource exe/fstl-e.rc)

#set Policy CMP0072 FindOpenGL behavior
set(OpenGL_GL_PREFERENCE GLVND)

#find required packages. 
find_package(Qt5 5.12 REQUIRED COMPONENTS Core Gui Widgets OpenGL)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

#add resources to RCC
qt5_add_resources(Project_Resources_RCC ${Project_Resources})

if(LINUX)
    find_program(XDG_MIME_BIN xdg-mime REQUIRED)
    find_program(XDG_DESKTOP_MENU_BIN xdg-desktop-menu REQUIRED)
    find_program(XDG_ICON_RESOURCE_bin xdg-icon-resource REQUIRED)
    set(XDG_Resources xdg/)
endif(LINUX)


#tell CMake AUTOGEN to skip autogen on the generated qrc files
set_property(SOURCE ${Project_Resources_RCC} PROPERTY SKIP_AUTOGEN ON)

#include opengl files. 
include_directories(${QT_QTOPENGL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} )

if(WIN32)
  add_executable(fstl-e WIN32 ${Project_Sources} ${Project_Headers} ${Project_Resources_RCC} ${Icon_Resource})
  set(Fstle_LINK_FLAGS ${CMAKE_CURRENT_SOURCE_DIR}/${Icon_Resource})
  set_target_properties(fstl-e PROPERTIES LINK_FLAGS ${Fstle_LINK_FLAGS})
elseif(APPLE)
  add_executable(fstl-e MACOSX_BUNDLE ${Project_Sources} ${Project_Headers} ${Project_Resources_RCC} ${Icon_Resource})
else()
  add_executable(fstl-e ${Project_Sources} ${Project_Headers} ${Project_Resources_RCC} ${Icon_Resource})
endif(WIN32)

target_link_libraries(fstl-e Qt5::Widgets Qt5::Core Qt5::Gui Qt5::OpenGL ${OPENGL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

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
        
        install(FILES
            $<TARGET_FILE:Qt5::Gui_EGL>
            $<TARGET_FILE:Qt5::Gui_GLESv2>
            $<TARGET_FILE:Qt5::Core>
            $<TARGET_FILE:Qt5::Gui> 
            $<TARGET_FILE:Qt5::OpenGL>
            $<TARGET_FILE:Qt5::Widgets>
            DESTINATION bin COMPONENT all)
        
        #install file in the platforms directory.
        install (FILES
            ${Qt5Core_DIR}/../../../plugins/platforms/qwindows.dll
            DESTINATION bin/platforms COMPONENT all
        )

        #custom commands based on: https://gist.github.com/Rod-Persky/e6b93e9ee31f9516261b
        add_custom_command(TARGET fstl-e POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core>       $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui>        $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Widgets>    $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::OpenGL>     $<TARGET_FILE_DIR:${PROJECT_NAME}>
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

    set(CPACK_PACKAGE_CONTACT "William Daniau")
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
    set(CPACK_GENERATOR "TGZ;DEB")
    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
endif(WIN32)

include(CPack)
