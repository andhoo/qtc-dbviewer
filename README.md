# Qt Creator SQL Database Viewer Plugin

## Introduction

Plugin allows to view databases supported by QtSql module.

## Features

1. Add several database connections with different drivers or connection options.
2. Browse tables list, table's data and schema.
3. Execute SQL queries.
4. Copy table's or query's contents to clipboard.
5. Add, edit, and delete rows.
6. Sort columns by clicking the header.
7. Export query results to HTML.

## Supported Databases

The plugin supports all QtSql drivers including:
- SQLite 3.x
- MySQL 4.x / 5.x
- PostgreSQL 8.x
- ODBC
- IBM DB2
- Oracle Call Interface
- Borland InterBase
- Sybase Adaptive Server

## Usage

### Opening the Plugin

The database viewer appears as a database icon in Qt Creator's left sidebar. Click on it to open the viewer.

### Adding a Database Connection

1. Right-click in the left tree view (database list)
2. Select "Add Connection" from the context menu
3. Choose a database driver from the dropdown
4. Fill in connection details:
   - **Label**: Friendly name for the connection
   - **Type**: Database driver (e.g., QSQLITE, QMYSQL, QPSQL, QODBC)
   - **Hostname**: Server address (not needed for SQLite)
   - **Port**: Server port (default varies by driver)
   - **Username**: Database user
   - **Password**: Database password
   - **Database**: Database name or SQLite filename
5. Click "Test" to verify the connection
6. Click "OK" to save

### Browsing Tables

1. Click the database icon in the tree to expand it
2. Click on a table name to view its data in the right pane
3. Double-click column headers to sort

### Running SQL Queries

1. Click the "Query" tab
2. Type your SQL query in the editor
3. Click "Go" or press the query button to execute
4. Results appear in the table below
5. Use "Save Query Data" to export to HTML

### Copying Data

- Select cells in the data or query table
- Click "Copy Data" or "Copy Query Data" button
- Data is copied to clipboard as tab-separated values
- Paste into Excel or other applications

### Managing Data

- **Add Row**: Click "Add Row" button
- **Delete Row**: Select rows and click "Delete Row"
- **Save Changes**: Click "Save Data" to submit changes
- **Revert Changes**: Click "Revert Data" to undo

### Keyboard Shortcuts

Buttons in the plugin have keyboard shortcuts registered with Qt Creator. Hover over a button to see its shortcut.

## Limitations

- **No menu bar**: The plugin uses a context menu (right-click) for actions
- **No toolbar**: All actions are accessible via context menus or buttons
- **Add Connection**: Only available via right-click on the tree view

## Acknowledgements

Plugin based on Timo Bingmann's QtSqlView application.

### Icon Credits

UI icons restored from original QtSqlView application (Qtc 5.0 commit).

## Requirements

- Qt6
- Qt Creator 19.0.0+
- C++20 compiler
- MSVC 2022 (recommended on Windows)

## Installation

IMPORTANT: plugin's version must match Qt Creator's version (difference in last digit is acceptable)

### From source

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

### From binaries

1. Copy the built plugin DLL to Qt Creator's plugins directory.
2. Enable the `QtcDbViewer` plugin (Help → About Plugins...)
