#include "QtcDbViewerPlugin.h"
#include "Constants.h"
#include "QtcDbViewerTr.h"
#include "WMain.h"

#include <QAction>
#include <QToolButton>

#include <coreplugin/icore.h>
#include <coreplugin/imode.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <utils/icon.h>

namespace QtcDbViewer::Internal {

const Utils::Icon MODE_DATABASE_CLASSIC(
    Utils::FilePath::fromString(QLatin1String(":/icons/database.png")));

static QList<Utils::IconMaskAndColor> flatIconData()
{
    return {{Utils::FilePath::fromString(QLatin1String(":/icons/database_mask.png")), Utils::Theme::IconsBaseColor}};
}

const Utils::Icon MODE_DATABASE_FLAT(flatIconData());

QtcDbViewerPlugin::QtcDbViewerPlugin() = default;

QtcDbViewerPlugin::~QtcDbViewerPlugin() = default;

void QtcDbViewerPlugin::initialize()
{
    auto context = Core::Context(Constants::QTCDBVIEWER_CONTEXT);

    Core::IMode *dbViewMode = new Core::IMode(this);
    dbViewMode->setId(Constants::QTCDBVIEWER_ID);
    dbViewMode->setContext(context);
    dbViewMode->setDisplayName(Tr::tr("Db Viewer"));
    dbViewMode->setIcon(Utils::Icon::sideBarIcon(MODE_DATABASE_CLASSIC, MODE_DATABASE_FLAT));
    dbViewMode->setPriority(10);
    dbViewMode->setWidgetCreator([context]() -> QWidget * {
        auto viewer = new WMain;
        auto buttons = viewer->findChildren<QToolButton *>();
        for (auto button : buttons) {
            if (button->toolTip().isEmpty()) {
                continue;
            }
            auto *proxyAction = new QAction(button->toolTip(), button);
            QObject::connect(proxyAction, &QAction::triggered, button, &QToolButton::click);

            QString idString = QStringLiteral("QtcDbViewer.") + button->objectName();
            auto id = Utils::Id::fromString(idString);
            auto *cmd = Core::ActionManager::registerAction(proxyAction, id, context);
            cmd->setDefaultKeySequence(button->shortcut().toString());
            button->setShortcut(QKeySequence());
            QObject::connect(cmd, &Core::Command::keySequenceChanged, [button, proxyAction, cmd]() {
                QString tooltip = proxyAction->text();
                if (!cmd->keySequence().isEmpty()) {
                    tooltip += QString(QStringLiteral(" (%1)")).arg(cmd->keySequence().toString());
                }
                button->setToolTip(tooltip);
            });
            cmd->keySequenceChanged();
        }
        return viewer;
    });
}

void QtcDbViewerPlugin::extensionsInitialized()
{
}

ExtensionSystem::IPlugin::ShutdownFlag QtcDbViewerPlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}

} // namespace QtcDbViewer::Internal

#include <QtcDbViewerPlugin.moc>