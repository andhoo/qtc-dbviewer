#pragma once

#include "PluginGlobal.h"
#include "QtcDbViewerTr.h"

#include <extensionsystem/iplugin.h>

namespace QtcDbViewer::Internal {

class QtcDbViewerPlugin final : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "QtcDbViewer.json")

public:
    QtcDbViewerPlugin();

    ~QtcDbViewerPlugin() final;

    void initialize() final;

    void extensionsInitialized() final;

    ExtensionSystem::IPlugin::ShutdownFlag aboutToShutdown() final;

private:
    void triggerAction();
};

} // namespace QtcDbViewer::Internal

