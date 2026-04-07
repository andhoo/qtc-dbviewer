#include "QtcDbViewerPlugin.h"
#include "Constants.h"
#include "WMain.h"

#include <QCoreApplication>
#include <QTranslator>

#include <QtPlugin>

#include <coreplugin/icore.h>
#include <coreplugin/imode.h>
#include <utils/icon.h>
#include <utils/filepath.h>
#include <coreplugin/actionmanager/actionmanager.h>

using namespace QtcDbViewer::Internal;

const Utils::Icon MODE_DATABASE_CLASSIC (
  Utils::FilePath::fromString (QLatin1String (":/icons/database.png")));

static QList<Utils::IconMaskAndColor> flatIconData () {
  return {{Utils::FilePath::fromString (QLatin1String (":/icons/database_mask.png")), Utils::Theme::IconsBaseColor}};
}

const Utils::Icon MODE_DATABASE_FLAT (flatIconData ());

QtcDbViewerPlugin::QtcDbViewerPlugin () :
  IPlugin () {
}

QtcDbViewerPlugin::~QtcDbViewerPlugin () {
}

bool QtcDbViewerPlugin::initialize (const QStringList &arguments, QString *errorString) {
  Q_UNUSED (arguments)
  Q_UNUSED (errorString)

  initLanguage ();

  auto context = Core::Context (Constants::QTCDBVIEWER_CONTEXT);

  auto viewer = new WMain;
  auto buttons = viewer->findChildren<QToolButton *>();
  for (auto button: buttons) {
    if (button->toolTip ().isEmpty ()) {
      continue;
    }
    auto *proxyAction = new QAction (button->toolTip (), button);
    connect (proxyAction, SIGNAL (triggered (bool)), button, SIGNAL (clicked (bool)));

    auto id = Utils::Id::fromString (QStringLiteral ("QtcDbViewer.") + button->objectName ());
    auto *cmd = Core::ActionManager::registerAction (proxyAction, id, context);
    cmd->setDefaultKeySequence (button->shortcut ().toString ());
    button->setShortcut (QString ());
    connect (cmd, &Core::Command::keySequenceChanged, [button, proxyAction, cmd]() {
      QString tooltip = proxyAction->text ();
      if (!cmd->keySequence ().isEmpty ()) {
        tooltip += QString (QStringLiteral (" (%1)")).arg (cmd->keySequence ().toString ());
      }
      button->setToolTip (tooltip);
    });
    cmd->keySequenceChanged ();
  }

  Core::IMode *dbViewMode = new Core::IMode(this);
  dbViewMode->setId (Constants::QTCDBVIEWER_ID);
  dbViewMode->setContext (context);
  dbViewMode->setDisplayName (tr ("Db Viewer"));
  dbViewMode->setIcon (Utils::Icon::sideBarIcon (MODE_DATABASE_CLASSIC,
                                                 MODE_DATABASE_FLAT));
  dbViewMode->setPriority (10);
  dbViewMode->setWidget (viewer);

  return true;
}

void QtcDbViewerPlugin::initLanguage () {
  const QString &language = Core::ICore::userInterfaceLanguage ();
  if (!language.isEmpty ()) {
    QStringList paths;
    paths << Core::ICore::resourcePath ().toUrlishString ()
          << Core::ICore::userResourcePath ().toUrlishString ();
    const QString &trFile = QLatin1String ("QtcDbViewer_") + language;
    QTranslator *translator = new QTranslator (this);
    foreach (const QString &path, paths) {
      if (translator->load (trFile, path + QLatin1String ("/translations"))) {
        qApp->installTranslator (translator);
        break;
      }
    }
  }
}

void QtcDbViewerPlugin::extensionsInitialized () {
}

ExtensionSystem::IPlugin::ShutdownFlag QtcDbViewerPlugin::aboutToShutdown () {
  return SynchronousShutdown;
}
