#include "pluginexampleconstants.h"
#include "pluginexampletr.h"

#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/icontext.h>
#include <coreplugin/icore.h>
#include <coreplugin/imode.h>
#include <coreplugin/modemanager.h>

#include <extensionsystem/iplugin.h>

#include <QAction>
#include <QIcon>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>

using namespace Core;

namespace Pluginexample::Internal {

class PluginexamplePlugin final : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Pluginexample.json")

public:
    PluginexamplePlugin();

    ~PluginexamplePlugin() final;

    void initialize() final;

    void extensionsInitialized() final;

    ExtensionSystem::IPlugin::ShutdownFlag aboutToShutdown() final;

private:
    Core::IMode *m_mode = nullptr;

    void triggerAction();
};

PluginexamplePlugin::PluginexamplePlugin() = default;

PluginexamplePlugin::~PluginexamplePlugin() = default;

void PluginexamplePlugin::initialize()
{
    ActionContainer *menu = ActionManager::createMenu(Constants::MENU_ID);
    menu->menu()->setTitle(Tr::tr("Pluginexample"));
    ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    ActionBuilder(this, Constants::ACTION_ID)
        .addToContainer(Constants::MENU_ID)
        .setText(Tr::tr("Pluginexample Action"))
        .setDefaultKeySequence(Tr::tr("Ctrl+Alt+Meta+A"))
        .addOnTriggered(this, &PluginexamplePlugin::triggerAction);

    const QIcon modeIcon = QIcon::fromTheme("document");
    m_mode = new Core::IMode;
    m_mode->setId(Constants::MODE_ID);
    m_mode->setDisplayName(Tr::tr("Pluginexample"));
    m_mode->setIcon(modeIcon);
    m_mode->setWidgetCreator([]() -> QWidget * {
        return new QWidget;
    });
}

void PluginexamplePlugin::extensionsInitialized()
{
}

ExtensionSystem::IPlugin::ShutdownFlag PluginexamplePlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}

void PluginexamplePlugin::triggerAction()
{
    QMessageBox::information(ICore::dialogParent(),
                             Tr::tr("Action Triggered"),
                             Tr::tr("This is an action from Pluginexample."));
}

} // namespace Pluginexample::Internal

#include <pluginexample.moc>
