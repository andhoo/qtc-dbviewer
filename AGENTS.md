# AGENTS.md - Qt Creator Database Viewer Plugin

## Project Overview

This is a Qt Creator plugin that provides SQL database browsing functionality.
It allows viewing databases supported by QtSql module, including browsing tables,
executing SQL queries, and copying data to clipboard.

## Build Commands

### Prerequisites
- Qt6 (Core, Gui, Widgets, Sql)
- Qt Creator 19.0.0+
- C++20 compiler

### Build with CMake
```bash
# Linux/macOS
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH="/path/to/QtCreator" ..
cmake --build .

# Windows (MSVC)
mkdir build && cd build
cmake -G "NMake Makefiles" -DCMAKE_PREFIX_PATH="C:/Qt/Tools/QtCreator" ..
cmake --build .
```

### Running a Single Test
No tests are currently defined in this project.

### Code Formatting
```bash
# Using uncrustify
uncrustify -c uncrustify.cfg -f src/file.cpp -o src/file.cpp
```

## Code Style Guidelines

### General Rules
- **C++ Standard**: C++20
- **Indentation**: 2 spaces (no tabs)
- **Line Endings**: Auto-detect (LF/CRLF)
- **Encoding**: UTF-8

### Naming Conventions
- **Classes**: PascalCase (e.g., `QtcDbViewerPlugin`)
- **Functions**: PascalCase (e.g., `initialize()`)
- **Variables**: camelCase (e.g., `viewer`, `context`)
- **Constants**: UPPER_SNAKE_CASE with `k` prefix for compile-time (e.g., `kMaxConnections`)
- **Headers**: Lowercase with underscore (e.g., `constants.h`)

### Formatting Rules (uncrustify)
Follow `uncrustify.cfg` configuration:
- Braces: BSD style (newline before opening brace)
- `if/for/while`: `if (condition) {` (space after keyword)
- Pointer/References: `Type* name` (no space before `*`, space after)
- Operators: `a + b`, `a && b`, `a == b` (spaces around operators)
- Control statements: `case`, `switch` indent 2 spaces

### Spacing Examples
```cpp
// Correct
void initialize() {
    auto viewer = new WMain;
    auto buttons = viewer->findChildren<QToolButton *>();
    for (auto button : buttons) {
        if (button->toolTip().isEmpty()) {
            continue;
        }
    }
}

// Incorrect
void initialize (){
    auto viewer = new WMain;
    auto buttons = viewer->findChildren<QToolButton*>();
    if(button->toolTip().isEmpty()){ continue; }
}
```

### Import Order
1. Project headers (local includes)
2. Plugin-specific headers (QtcDbViewerTr.h, Constants.h)
3. Qt Creator framework headers (coreplugin/, extensionsystem/)
4. Qt framework headers (<Q*>)

```cpp
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
```

### Namespace Usage
- Use namespace for plugin-specific code: `namespace QtcDbViewer { namespace Internal { ... } }`
- Namespace body is indented (2 spaces per level)

### Error Handling
- Use `Q_UNUSED()` for unused parameters
- Use Qt's error reporting (qWarning, qCritical) for logging

### Qt-Specific Guidelines
- Use `QLatin1String()` for string literals in performance-critical code
- Prefer `QStringLiteral()` over `QLatin1String()` in template code
- Use `auto` for variable declarations when type is obvious
- Prefer `const` and `constexpr` where applicable

### UI Code
- Use `tr()` for user-facing strings
- Use `QLatin1String()` for internal strings
- Follow Qt Creator's action registration pattern

### File Organization
- Header files: `src/*.h`
- Implementation: `src/*.cpp`
- Resources: `resources/icons.qrc`
- Translations: `translation/*.ts`

### Resource Files
All icons are defined in `resources/icons.qrc`. This file is compiled into the plugin binary.

To ensure icons are available when the plugin loads, call `Q_INIT_RESOURCE(icons)` in the constructor of any class that uses icons from the resource file:
```cpp
WConnection(QWidget *parent, const DbParameter *dbp)
    : QDialog(parent) {
    setupUi(this);
    Q_INIT_RESOURCE(icons);
    // ...
}
```

Icon path format in code: `:/icons/filename.png` (e.g., `:/icons/table.png`)

### Qt Creator Plugin Specifics
- Inherit from `ExtensionSystem::IPlugin`
- Add `Q_OBJECT` macro in plugin class
- Add `Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "QtcDbViewer.json")` in header
- Implement `initialize()`, `extensionsInitialized()`, `aboutToShutdown()`
- Use `Core::Context` for action contexts
- Register actions via `Core::ActionManager`
- Use `Core::IMode` for adding modes to Qt Creator
- Use `setWidgetCreator(lambda)` instead of deprecated `setWidget()`

## UI Overview

### Main Widget (WMain)
The main plugin widget with three main areas:
- **Left**: Tree view showing database connections and tables (`treeDbList`)
- **Right**: Tab widget with three tabs:
  - **Table**: Shows table data (`dataTable`)
  - **Schema**: Shows table schema (`schemaTable`)
  - **Query**: SQL query editor (`editQuery`, `queryTable`)

### Connection Dialog (WConnection)
Dialog for creating/editing database connections. Fields:
- Label, Type (driver), Hostname, Port, Username, Password
- Database name/filename, Options
- Buttons: Test, OK, Cancel

### Context Menu Actions
Actions available via right-click:
- **Add Connection**: Create new database connection
- **Edit Connection**: Modify existing connection
- **Remove Connection**: Delete connection
- **Refresh Tablelist**: Refresh the table list
- **Add Row / Delete Row**: Data manipulation
- **Copy Data**: Copy selected cells to clipboard
- **Save/Revert Data**: Submit or undo changes

## Widget Classes

### DbConnection (src/DbConnection.h)
Manages individual database connections using QSqlDatabase.

### DbList (src/DbTypes.h)
Model class that holds list of DbConnection objects and provides tree structure.

### DbSchemaModel (src/DbTypes.h)
Model for displaying table schema information.

### SQLHighlighter (src/SQLHighlighter.h)
Syntax highlighter for SQL query editor.

## Key Implementation Details

### Button Registration
The plugin automatically registers all QToolButton widgets in WMain as Qt Creator actions:
```cpp
auto viewer = new WMain;
auto buttons = viewer->findChildren<QToolButton *>();
for (auto button : buttons) {
    if (button->toolTip().isEmpty()) {
        continue;
    }
    auto *proxyAction = new QAction(button->toolTip(), button);
    QObject::connect(proxyAction, &QAction::triggered, button, &QToolButton::click);
    // Register with ActionManager...
}
```

### Mode Creation
Creates a Qt Creator mode (sidebar entry):
```cpp
Core::IMode *dbViewMode = new Core::IMode(this);
dbViewMode->setId(Constants::QTCDBVIEWER_ID);
dbViewMode->setWidgetCreator([context]() -> QWidget * { return new WMain; });
```

### Data Copy to Clipboard
Data is copied as tab-separated values for easy pasting into Excel:
```cpp
QString seltext;
for (int rowi = selrange.top(); rowi <= selrange.bottom(); ++rowi) {
    for (int fi = selrange.left(); fi <= selrange.right(); ++fi) {
        if (fi != selrange.left()) seltext += "\t";
        seltext += query.value(fi).toString();
    }
    seltext += "\n";
}
QClipboard *clip = QApplication::clipboard();
clip->setText(seltext, mode);
```

## Historical Notes

- Based on Timo Bingmann's QtSqlView standalone application
- Originally used `setWidget()` (deprecated in favor of `setWidgetCreator()`)
- No menu bar or toolbar - uses context menus instead
- Uses BSD-style formatting with 2-space indentation
- Originally used SIGNAL/SLOT macros, converted to modern pointer syntax
