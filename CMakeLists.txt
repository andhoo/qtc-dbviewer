cmake_minimum_required(VERSION 3.16)

# Remove when sharing with others.
list(APPEND CMAKE_PREFIX_PATH "C:/Qt/Tools/QtCreator")

project(QtcDbViewer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(QtCreator REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

option(WITH_TESTS "Builds with tests" NO)

if(WITH_TESTS)
  find_package(Qt6 REQUIRED COMPONENTS Test)
  set(IMPLICIT_DEPENDS Qt::Test)
  enable_testing()
endif()

add_qtc_plugin(QtcDbViewer
  PLUGIN_DEPENDS
    QtCreator::Core
  DEPENDS
    Qt::Widgets
    Qt::Sql
    QtCreator::ExtensionSystem
    QtCreator::Utils
  SOURCES
    src/Constants.h
    src/PluginGlobal.h
    src/QtcDbViewerPlugin.h
    src/QtcDbViewerPlugin.cpp
    src/QtcDbViewerTr.h
    src/DbConnection.h
    src/DbConnection.cpp
    src/DbTypes.h
    src/SQLHighlighter.h
    src/SQLHighlighter.cpp
    src/WConnection.h
    src/WConnection.ui
    src/WMain.h
    src/WMain.ui
    translation/QtcDbViewer_ru.ts
    resources/icons.qrc
    CMakeLists.txt
    QtcDbViewer.json.in
    README.md
    LICENSE.md
    AGENTS.md
    uncrustify.cfg
)

# Enable the Run button in Qt Creator
get_target_property(QtCreatorCorePath QtCreator::Core LOCATION)
find_program(QtCreatorExecutable
  NAMES
    qtcreator "Qt Creator"
  PATHS
    "${QtCreatorCorePath}/../../../bin"
    "${QtCreatorCorePath}/../../../MacOS"
  NO_DEFAULT_PATH
)
if(QtCreatorExecutable)
  add_custom_target(RunQtCreator
    COMMAND ${QtCreatorExecutable} -pluginpath $<TARGET_FILE_DIR:QtcDbViewer>
    DEPENDS QtcDbViewer
  )
  set_target_properties(RunQtCreator PROPERTIES FOLDER "qtc_runnable")
endif()