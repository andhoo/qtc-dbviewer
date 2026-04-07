# QtcDbViewer Plugin - Build Samenvatting

## Project Status

| Aspect | Status | Opmerking |
|--------|--------|-----------|
| **Qt Versie** | 6.10.1 | MinGW compiler |
| **Qt Creator** | 19.0.0 | Via MaintenanceTool geïnstalleerd |
| **Code Port** | ✅ Gedeeltelijk | Qt6 + Qt Creator 19 API fixes toegepast |
| **CMakeLists.txt** | ✅ Klaar | Alternatief voor qmake |
| **Build met MinGW** | ❌ Werkt niet | MinGW kan niet linken met MSVC Qt Creator DLLs |

---

## Wat Werkt

### 1. Code API Fixes (已完成 / Done)

| Bestand | Fix | Status |
|---------|-----|--------|
| `DbConnection.h` | `Utils::Key` API aangepast | ✅ |
| `QtcDbViewerPlugin.cpp` | `toString()` → `toUrlishString()` | ✅ |
| `QtcDbViewerPlugin.h` | Metadata verwijderd voor MOC | ✅ |
| `SQLHighlighter.h/cpp` | QRegExp → QRegularExpression | ✅ |
| `WMain.h` | QVariant::type() → typeId() | ✅ |
| `qtc-dbviewer.pro` | Q_PLUGIN_METADATA verwijderd | ✅ |

### 2. Import Library poging

| Stap | Status |
|------|--------|
| gendef.exe draaien op DLLs | ✅ Werkte (met warnings) |
| dlltool.exe voor .a files | ✅ libExtensionSystem.a, libUtils.a, libCore.a aangemaakt |
| Linken met MinGW | ❌ Faalde door ABI mismatch |

---

## Wat Niet Werkt

### Command-line Build met MinGW

**Probleem:** Qt Creator 19 is gebouwd met MSVC. MinGW kan niet linken tegen MSVC DLLs.

**Foutmeldingen:**
```
undefined reference to `__imp__ZN5Utils3KeyD1Ev'
undefined reference to `__imp__ZN15ExtensionSystem7IPlugin16staticMetaObjectE'
... (30+ meer undefined references)
```

### Root Cause: C++ ABI Incompatibiliteit

MSVC en MinGW gebruiken verschillende C++ name mangling schemes:

| Compiler | Mangled Name Voorbeeld |
|----------|------------------------|
| MSVC | `??0IPlugin@ExtensionSystem@@QEAA@XZ` |
| MinGW (Itanium) | `_ZN15ExtensionSystem7IPluginC1Ev` |

**Dit lost gendef/dlltool niet op:**
- Import libraries kunnen wel gemaakt worden
- Maar de symbolen komen niet overeen
- Linker kan de functies niet vinden

### Tools die we probeerden

| Tool | Resultaat |
|------|-----------|
| `gendef` + `dlltool` | Maakt import library, maar verkeerde symbolen |
| `implib-sdk` | zelfde probleem |
| `llvm-cxxmap` | Werkt alleen Itanium ↔ Itanium |
| Mixed toolchain (MinGW compile + MSVC link) | Symbol mismatch |
| Assembly shim | Onmogelijk voor C++ classes |
| llvm-mingw (clang) | Nog steeds Itanium ABI |

---

## Oplossingen Die Wel Werken

### Optie A: MSVC Build Tools (Aanbevolen voor command-line)

1. Download: https://visualstudio.microsoft.com/visual-cpp-build-tools/
2. Kies "Build Tools for Visual Studio 2022"
3. Installeer "MSVC v143 - VS 2022 C++ x64/x86 build tools"
4. Gebruik `cl.exe` en `link.exe` voor build

**Voordelen:**
- Werkt gegarandeerd met Qt Creator 19
- Command-line build mogelijk

**Nadelen:**
- ~3GB download
- MSVC compiler nodig

---

### Optie B: Qt Creator Broncode Bouwen

1. Download Qt Creator source: `git clone https://code.qt.io/qt-creator/qt-creator.git`
2. Bouw met MinGW
3. Plugin linken tegen eigen Qt Creator libs

**Voordelen:**
- Alles consistent (MinGW ↔ MinGW)
- Volledige controle

**Nadelen:**
- Complex (uren bouwen)
- Qt Creator ~1GB+ om te bouwen

---

### Optie C: Standalone App (IPC)

Maak een standalone applicatie die communiceert met Qt Creator via:

- **HTTP/REST:** Je app draait als webserver, Qt Creator opent URL
- **Named Pipes:** Windows IPC
- **File-based:** Exporteer naar JSON/CSV, Qt Creator toont het

**Voordelen:**
- Geen linking issues
- Elke taal mogelijk (Python, C++, Rust)

**Nadelen:**
- Geen native integratie in Qt Creator UI
- Zelf UI bouwen

---

## Overzicht Gewijzigde Bestanden

### Gewijzigd (Code Fixes):

```
src/QtcDbViewerPlugin.cpp    - toUrlishString() fix
src/QtcDbViewerPlugin.h      - Metadata verwijderd
3rd-party/QtSqlView/src/DbConnection.h   - Utils::Key fix
3rd-party/QtSqlView/src/WMain.h          - QVariant fix
3rd-party/QtSqlView/src/SQLHighlighter.h - QRegularExpression
3rd-party/QtSqlView/src/SQLHighlighter.cpp - QRegularExpression
qtc-dbviewer.pro              - Q_PLUGIN_METADATA verwijderd
paths.pri                     - Import library paden (werkt niet)
```

### Nieuw Aangemaakt:

```
CMakeLists.txt               - CMake build configuratie
QtcDbViewer.json             - Plugin metadata
build/import_libs/          - Gegenereerde import libraries (niet werkend)
```

---

## Relevante Directories

```
D:\Users\andre\Projects\Qt\qtc-dbviewer\
├── build/import_libs/       - Import library poging
│   ├── libExtensionSystem.a
│   ├── libUtils.a
│   └── libCore.a
├── src/
│   └── QtcDbViewerPlugin.cpp
└── 3rd-party/QtSqlView/src/
    ├── DbConnection.h
    ├── WMain.h
    └── SQLHighlighter.cpp
```

---

## Beslissing

**Build met MinGW is niet mogelijk** zonder MSVC te installeren of Qt Creator zelf te bouwen.

De eenvoudigste oplossing: **MSVC Build Tools** installeren en de plugin daarmee bouwen.

---

## Contact

Plugin auteur: Gres  
Project URL: http://gres.biz/qtc-dbviewer  
Licentie: MIT License

---

*Laatst bijgewerkt: 2026-04-07*
*Technische notes: C++ ABI conflict tussen MSVC en MinGW is onoverkomelijk zonder één van beide toolchains te gebruiken*