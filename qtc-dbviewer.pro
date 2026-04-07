DEFINES += QTCDBVIEWER_LIBRARY

VERSION_SUFFIX = ""

include(paths.pri)

# Qt Creator include paths
INCLUDEPATH += C:/Qt/Tools/QtCreator/include/qtcreator/src/libs
INCLUDEPATH += C:/Qt/Tools/QtCreator/include/qtcreator/src/plugins
INCLUDEPATH += C:/Qt/Tools/QtCreator/include/qtcreator/src/share

# Build as shared library (plugin)
TEMPLATE = lib
CONFIG += plugin

# Qt Creator lib paths - use MinGW import libs
# (MSVC .lib files are removed - using custom generated .a files from paths.pri)

# Add -Wl,--enable-auto-import for implicit DLL linking
QMAKE_LFLAGS += -Wl,--enable-auto-import

# QtcDbViewer files

include (3rd-party/QtSqlView/src/src.pri)

SOURCES += \
    src/QtcDbViewerPlugin.cpp

HEADERS += \
    src/Constants.h \
    src/PluginGlobal.h \
    src/QtcDbViewerPlugin.h

TRANSLATIONS += \
    translation/QtcDbViewer_ru.ts

RESOURCES += \
    resources/resources.qrc

OTHER_FILES += \
    LICENSE.md \
    README.md \
    uncrustify.cfg \
    dist/README.md \
    dist/make_dist.bat \
    dist/make_dist.sh \
    resources/README.md

OTHER_FILES += $$TRANSLATIONS

PROVIDER = Gres

###### If the plugin can be depended upon by other plugins, this code needs to be outsourced to
###### <dirname>_dependencies.pri, where <dirname> is the name of the directory containing the
###### plugin's sources.

QTC_PLUGIN_NAME = QtcDbViewer
QTC_LIB_DEPENDS += \
    # nothing here at this time

QTC_PLUGIN_DEPENDS += \
    coreplugin

QTC_PLUGIN_RECOMMENDS += \
    # optional plugin dependencies. nothing here at this time

###### End _dependencies.pri contents ######

# Plugin metadata is defined in the source file