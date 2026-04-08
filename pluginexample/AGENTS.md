# AGENTS.md - Development Guidelines for Pluginexample

## Project Overview

This is a Qt Creator plugin written in C++20 using CMake as the build system. The plugin adds a menu action to Qt Creator.

## Build Commands

### Local Build (Windows)

```bash
# Create build directory
mkdir build && cd build

# Configure (adjust CMAKE_PREFIX_PATH to your Qt Creator installation)
cmake -DCMAKE_PREFIX_PATH="C:/Qt/Tools/QtCreator" -DCMAKE_BUILD_TYPE=RelWithDebInfo ..

# Build
cmake --build .
```

### Build with Tests

```bash
cmake -DWITH_TESTS=ON -DCMAKE_PREFIX_PATH="<path_to_qtcreator>" ..
cmake --build .

# Run tests
ctest
```

### Run Single Test

```bash
# After building with tests enabled
ctest -R <test_name> -V
```

### Qt Creator Development Build

```bash
# Build and run Qt Creator with the plugin
cmake --build . --target RunQtCreator
```

## Code Style Guidelines

### C++ Standards
- Use C++20 features (modern, clean)
- Use `final` keyword on classes and methods when not intended for override
- Prefer `= default` or `= delete` for special member functions
- Use `[[nodiscard]]`, `[[maybe_unused]]` attributes where appropriate

### Naming Conventions
- **Classes/Structs**: UpperCamelCase (e.g., `PluginexamplePlugin`)
- **Methods/Functions**: UpperCamelCase (e.g., `initialize()`, `triggerAction()`)
- **Variables**: lowerCamelCase (e.g., `menu`, `qtCreatorExecutable`)
- **Constants/Enums**: UpperCamelCase or UPPER_SNAKE_CASE for macros
- **Namespaces**: lowercase, e.g., `Pluginexample::Internal`

### Qt Conventions
- Include Qt headers with angle brackets: `<QAction>`
- Include QtCreator headers with angle brackets and full path: `<coreplugin/icore.h>`
- Use `Q_OBJECT` macro in classes that need signals/slots
- Use `Q_PLUGIN_METADATA` for plugin interface
- Use `Tr::tr()` for user-visible strings (translatable)
- Prefer Qt container classes (QList, QVector, QString) over STL equivalents in Qt code

### Includes Organization
Order (recommended):
1. Corresponding header (if implementing)
2. Plugin-specific headers (pluginexampleconstants.h)
3. Qt Creator framework headers (coreplugin/, extensionsystem/)
4. Qt framework headers (<Q*>)
5. Standard library (only if necessary)

Example:
```cpp
#include "pluginexampleconstants.h"

#include <coreplugin/actionmanager/actionmanager.h>
#include <extensionsystem/iplugin.h>

#include <QMessageBox>
```

### Error Handling
- Use Qt's error handling patterns (return values, nullptr checks)
- For recoverable errors, return early with appropriate logging
- Use `Q_CHECK_PTR` for critical null checks in low-level code
- Avoid exceptions in Qt code (Qt doesn't use them by default)

### Plugin Structure
Follow this pattern:
```cpp
namespace Pluginexample::Internal {

class MyPlugin final : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "PluginName.json")

public:
    MyPlugin();
    ~MyPlugin() final;

    void initialize() final;
    void extensionsInitialized() final;
    ShutdownFlag aboutToShutdown() final;

private:
    void doSomething();
};

} // namespace Pluginexample::Internal

#include <pluginname.moc>
```

### Actions and Menus
- Use `ActionBuilder` pattern for creating actions
- Define menu IDs and action IDs in `constants.h`
- Use translation function `Tr::tr()` for all user-visible text

### Testing (if enabled with WITH_TESTS)
- Use Qt Test framework (`QTest` + `Q_OBJECT` test class)
- Place tests in a `test/` subdirectory
- Test naming: `tst_<tested_class>.cpp`
- Test class naming: `<Class>Test`

### CI/CD
- GitHub Actions workflow: `.github/workflows/build_cmake.yml`
- Builds on Windows (MSVC), Linux (GCC), macOS (Clang)
- Uses Qt Creator's `build_plugin.py` script for building

### Qt Specific Notes
- This project uses CMAKE_AUTOMOC, CMAKE_AUTORCC, CMAKE_AUTOUIC
- MOC (Meta-Object Compiler) processing is automatic
- Ensure Q_OBJECT and Q_PLUGIN_METADATA are in header files processed by MOC
- Include `<filename.moc>` at end of .cpp files

## Common Tasks

### Adding a New Action
1. Define action ID in `pluginexampleconstants.h`
2. Create menu container in `initialize()`
3. Use ActionBuilder to register the action
4. Implement the action handler method

### Adding Dependencies
1. Add to `find_package()` in CMakeLists.txt
2. Add to `DEPENDS` in `add_qtc_plugin()`

### Debugging
- Use `qDebug()` for logging (works in Qt Creator)
- Use `--temporarycleansettings` (-tcs) when running Qt Creator during development