cmake_minimum_required(VERSION 3.16)
project(QtcDbViewer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt6 required components
set(QT_REQUIRED_COMPONENTS Core Gui Widgets Sql)
find_package(Qt6 REQUIRED COMPONENTS ${QT_REQUIRED_COMPONENTS})

# Set Qt plugin metadata path for MOC
set(QT_PLUGIN_METADATA_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/QtcDbViewer.json"
)

# Qt Creator paths
set(QTC_ROOT_DIR "C:/Qt/Tools/QtCreator")
set(QTC_PLUGINS_DIR "${QTC_ROOT_DIR}/lib/qtcreator/plugins")
set(QTC_BIN_DIR "${QTC_ROOT_DIR}/bin")

# Generate import libraries from Qt Creator DLLs if not exist
set(IMPORT_LIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/import_libs")
set(CUSTOM_IMPORT_LIBS 
    "${IMPORT_LIBS_DIR}/libExtensionSystem.a"
    "${IMPORT_LIBS_DIR}/libUtils.a"
)

# Check if custom import libs exist, if not they should be generated manually
# For now, we assume they exist from the gendef/dlltool process

# Source files
set(PLUGIN_SOURCES
    src/QtcDbViewerPlugin.cpp
    src/Constants.h
    src/PluginGlobal.h
    src/QtcDbViewerPlugin.h
)

set(THIRDPARTY_SOURCES
    3rd-party/QtSqlView/src/DbTypes.h
    3rd-party/QtSqlView/src/DbConnection.h
    3rd-party/QtSqlView/src/WConnection.h
    3rd-party/QtSqlView/src/WConnection.ui
    3rd-party/QtSqlView/src/WMain.h
    3rd-party/QtSqlView/src/WMain.ui
    3rd-party/QtSqlView/src/SQLHighlighter.h
    3rd-party/QtSqlView/src/SQLHighlighter.cpp
    3rd-party/QtSqlView/src/WMain.cpp
    3rd-party/QtSqlView/src/QtSqlView.qrc
    3rd-party/QtSqlView/src/resources.rc
)

set(PLUGIN_RESOURCES
    resources/resources.qrc
)

set(ALL_SOURCES
    ${PLUGIN_SOURCES}
    ${THIRDPARTY_SOURCES}
    ${PLUGIN_RESOURCES}
)

add_library(${PROJECT_NAME} MODULE ${ALL_SOURCES})

# Qt Creator include directories
set(QTC_INCLUDE_DIRS
    "${QTC_ROOT_DIR}/include/qtcreator/src/libs"
    "${QTC_ROOT_DIR}/include/qtcreator/src/plugins"
    "${QTC_ROOT_DIR}/include/qtcreator/src/share"
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${QTC_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/3rd-party/QtSqlView/src
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    QTCDBVIEWER_LIBRARY
    QT_QML_DEBUG
)

# Link Qt6
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
)

if(MINGW)
    # Use custom generated import libraries
    # Only use the ones that were successfully generated
    if(EXISTS "${IMPORT_LIBS_DIR}/libExtensionSystem.a")
        message(STATUS "Using custom import libraries from: ${IMPORT_LIBS_DIR}")
        target_link_libraries(${PROJECT_NAME} PRIVATE
            "${IMPORT_LIBS_DIR}/libExtensionSystem.a"
        )
        
        # Try to add Utils if it exists (may have issues)
        if(EXISTS "${IMPORT_LIBS_DIR}/libUtils.a" AND NOT "$ENV{SKIP_UTILS}" STREQUAL "1")
            target_link_libraries(${PROJECT_NAME} PRIVATE
                "${IMPORT_LIBS_DIR}/libUtils.a"
            )
        endif()
        
        # Try to add Core if it exists
        if(EXISTS "${IMPORT_LIBS_DIR}/libCore.a")
            target_link_libraries(${PROJECT_NAME} PRIVATE
                "${IMPORT_LIBS_DIR}/libCore.a"
            )
        endif()
        
        # Add runtime path so the plugin can find Qt Creator DLLs at runtime
        set(CMAKE_BUILD_RPATH "${QTC_BIN_DIR};${QTC_PLUGINS_DIR}")
        set(CMAKE_INSTALL_RPATH "${QTC_BIN_DIR};${QTC_PLUGINS_DIR}")
    else()
        message(WARNING "Custom import libraries not found. Run gendef/dlltool to generate them.")
    endif()
else()
    # Try QtCreator package for MSVC
    set(QtCreator_DIR "${QTC_ROOT_DIR}/lib/cmake/QtCreator")
    if(EXISTS ${QtCreator_DIR})
        find_package(QtCreator QUIET)
        if(QtCreator_FOUND)
            target_link_libraries(${PROJECT_NAME} PRIVATE
                QtCreator::Core
                QtCreator::ExtensionSystem
                QtCreator::Utils
            )
        endif()
    endif()
endif()

# Plugin metadata
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "QtcDbViewer"
    PREFIX ""
    SUFFIX ".dll"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/plugin"
    BUILD_RPATH "${CMAKE_BUILD_RPATH}"
    INSTALL_RPATH "${CMAKE_INSTALL_RPATH}"
)

# Install rules
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "${QTC_PLUGINS_DIR}" LIBRARY DESTINATION "${QTC_PLUGINS_DIR}")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/QtcDbViewer.json"
    "${CMAKE_CURRENT_BINARY_DIR}/QtcDbViewer.json"
    @ONLY
)

message(STATUS "=== QtcDbViewer Plugin Configuration ===")
message(STATUS "Qt Version: ${Qt6_VERSION}")
message(STATUS "Qt Creator: ${QTC_ROOT_DIR}")
message(STATUS "Custom Import Libs: ${IMPORT_LIBS_DIR}")
message(STATUS "Output: ${CMAKE_CURRENT_SOURCE_DIR}/build/plugin/QtcDbViewer.dll")
message(STATUS "==========================================")