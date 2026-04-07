## set the QTC_SOURCE environment variable to override the setting here
QTC_SOURCE = $$(QTC_SOURCE)
isEmpty(QTC_SOURCE):QTC_SOURCE = C:/Qt/Tools/QtCreator/include/qtcreator/src

QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES = C:/Qt/Tools/QtCreator/include/qtcreator/src

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE = C:/Qt/Tools/QtCreator

## Custom import libraries for MinGW - use build directory path
CUSTOM_IMPORT_LIBS = D:/Users/andre/Projects/Qt/qtc-dbviewer/build/import_libs
LIBS += $$CUSTOM_IMPORT_LIBS/libExtensionSystem.a
LIBS += $$CUSTOM_IMPORT_LIBS/libUtils.a
LIBS += $$CUSTOM_IMPORT_LIBS/libCore.a