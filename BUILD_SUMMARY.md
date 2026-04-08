# QtcDbViewer Plugin - Build Summary

## Project Status

| Aspect | Status |
|--------|--------|
| **Qt Versie** | 6.x |
| **Qt Creator** | 19.0.0+ |
| **Build System** | CMake |
| **C++ Standard** | C++20 |

## Build Commands

```bash
# Windows (MSVC)
mkdir build && cd build
cmake -G "NMake Makefiles" -DCMAKE_PREFIX_PATH="C:/Qt/Tools/QtCreator" ..
cmake --build .

# Linux/macOS
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH="/path/to/QtCreator" ..
cmake --build .
```

## Project Structuur

```
qtc-dbviewer/
├── CMakeLists.txt
├── QtcDbViewer.json.in
├── src/
│   ├── QtcDbViewerPlugin.h/cpp
│   ├── QtcDbViewerTr.h
│   ├── Constants.h
│   ├── PluginGlobal.h
│   └── WMain.*, WConnection.*, DbConnection.*, SQLHighlighter.*
├── resources/
└── translation/
```

## Wijzigingen t.o.v. Originele Plugin

- CMake vervangt qmake/.pro
- C++20 in plaats van C++17
- Modern Qt Creator 19.0.0+ API
- Sources verplaatst van 3rd-party naar src/
- Iconen hersteld in resources/icons.qrc

## Changelog

### 2026-04-08
- Iconen hersteld vanuit Qtc 5.0 commit (werden vermist bij CMake migratie)
- Toegevoegd: resources/icons.qrc met alle UI iconen

---

*Laatst bijgewerkt: 2026-04-08*