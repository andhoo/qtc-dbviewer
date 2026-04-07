# AGENTS.md - Qt Creator Database Viewer Plugin

## Project Overview

This is a Qt Creator plugin that provides SQL database browsing functionality.
It allows viewing databases supported by QtSql module, including browsing tables,
executing SQL queries, and copying data to clipboard.

## Build Commands

### Prerequisites
- Qt6 (Core, Gui, Widgets, Sql)
- Qt Creator sources (matching version)
- C++17 compiler

### Build with qmake (Primary)
```bash
# Linux/macOS
qmake qtc-dbviewer.pro
make -j$(nproc)

# Windows (MSVC)
qmake qtc-dbviewer.pro
nmake
```

### Build with CMake (Alternative)
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### CI Build Scripts
```bash
python util/ci/get_qt.py      # Download Qt
python util/ci/get_qtc.py    # Download Qt Creator
python util/ci/build.py      # Build plugin
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
- **C++ Standard**: C++17
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
void initialize(const QStringList &arguments, QString *errorString) {
    auto viewer = new WMain;
    auto buttons = viewer->findChildren<QToolButton *>();
    for (auto button : buttons) {
        if (button->toolTip().isEmpty()) {
            continue;
        }
    }
}

// Incorrect
void initialize ( const QStringList& arguments, QString* errorString ){
    auto viewer = new WMain;
    auto buttons = viewer->findChildren<QToolButton*>();
    if(button->toolTip().isEmpty()){ continue; }
}
```

### Import Order
1. Project headers (local includes)
2. Qt headers (`<Q...>`)
3. Qt Creator headers (`<coreplugin/...>`, `<utils/...>`)

```cpp
#include "QtcDbViewerPlugin.h"
#include "Constants.h"
#include "WMain.h"

#include <QCoreApplication>
#include <QTranslator>

#include <QtPlugin>

#include <coreplugin/icore.h>
#include <coreplugin/imode.h>
#include <utils/icon.h>
#include <coreplugin/actionmanager/actionmanager.h>
```

### Namespace Usage
- Use namespace for plugin-specific code: `namespace QtcDbViewer { namespace Internal { ... } }`
- Namespace body is indented (2 spaces per level)

### Error Handling
- Use `Q_UNUSED()` for unused parameters
- Return `false` on failure with error message via `errorString`
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
- Resources: `resources/*.qrc`
- Translations: `translation/*.ts`

### Qt Creator Plugin Specifics
- Inherit from `ExtensionSystem::IPlugin`
- Implement `initialize()`, `extensionsInitialized()`, `aboutToShutdown()`
- Use `Core::Context` for action contexts
- Register actions via `Core::ActionManager`
- Use `Core::IMode` for adding modes to Qt Creator
