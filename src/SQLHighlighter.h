// $Id: SQLHighlighter.h 20 2006-10-10 08:24:36Z tb $
// Copyright 2006 Timo Bingmann
//
// SQL syntax highlight plugin for QTextEdit
//

#pragma once

#include <QVector>
#include <QRegularExpression>

#include <QtGui/QSyntaxHighlighter>
#include <QtGui/QTextCharFormat>

class SQLHighlighter : public QSyntaxHighlighter {
  Q_OBJECT

  public:
    SQLHighlighter (class QTextDocument *parent = NULL);

  protected:
    virtual void highlightBlock (const QString &text);

  private:
    struct Rule {
      QRegularExpression pattern;
      QTextCharFormat format;

      inline Rule () {
      }

      inline Rule (QString p, QTextCharFormat f)
        : pattern (p, QRegularExpression::CaseInsensitiveOption), format (f) {
      }
    };

    QVector<Rule>   rules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat commentFormat;
};
