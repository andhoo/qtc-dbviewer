// $Id: DbConnection.cpp 20 2006-10-10 08:24:36Z tb $
// Copyright 2006 Timo Bingmann
//
// Implementation of DbConnection methods
//

#include "DbConnection.h"

QSqlError DbConnection::connect(class DbList &dblist)
{
    db = QSqlDatabase::addDatabase(dbparam.driver, dbuuid.toString());
    db.setHostName(dbparam.hostname);
    db.setPort(dbparam.port);
    db.setDatabaseName(dbparam.database);
    db.setUserName(dbparam.username);

    if (dbparam.askpassword) {
        bool ok = false;
        QString password = QInputDialog::getText(nullptr, tr("Password"),
                                                 tr("Password for %1:").arg(dbparam.username),
                                                 QLineEdit::Password, QString(), &ok);
        if (!ok) {
            connecterror = QSqlError("Password entry cancelled", QString(), QSqlError::ConnectionError);
            dblist.tablelist_seterror(*this, connecterror);
            return connecterror;
        }
        db.setPassword(password);
    } else {
        db.setPassword(dbparam.password);
    }

    if (!dbparam.connectOptions.isEmpty()) {
        db.setConnectOptions(dbparam.connectOptions);
    }

    if (!db.open()) {
        connecterror = db.lastError();
        dblist.tablelist_seterror(*this, connecterror);
        return connecterror;
    }

    dblist.tablelist_load(*this);
    return QSqlError();
}

void DbConnection::disconnect(class DbList &dblist)
{
    dblist.tablelist_clear(*this);
    db.close();
    db = QSqlDatabase();
    QString connectionName = dbuuid.toString();
    QSqlDatabase::removeDatabase(connectionName);
}