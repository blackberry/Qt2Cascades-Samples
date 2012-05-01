/****************************************************************************
 **
 ** Portions Copyright (C) 2012 Research In Motion Limited.
 ** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Research In Motion Ltd. (http://www.rim.com/company/contact/)
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the BB10 Platform and is derived
 ** from a similar file that is part of the Qt Toolkit.
 **
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Research In Motion, nor the name of Nokia
 **     Corporation and its Subsidiary(-ies), nor the names of its
 **     contributors may be used to endorse or promote products
 **     derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ****************************************************************************/

#ifndef REGEXP_HPP
#define REGEXP_HPP

#include <QtCore/QObject>
#include <QtCore/QRegExp>
#include <QtCore/QVector>

/**
 * The RegExp class encapsulates the processing of a regular expression and makes all input
 * and output parameters available to the UI via properties.
 * Internally it uses QRegExp to do the actual processing.
 */
class RegExp: public QObject
{
    Q_OBJECT

    // We basically forward all the properties of a QRegExp object here
    Q_PROPERTY(QString pattern READ pattern WRITE setPattern NOTIFY patternChanged)
    Q_PROPERTY(QString escapedPattern READ escapedPattern NOTIFY regexpChanged)
    Q_PROPERTY(bool caseSensitive READ caseSensitive WRITE setCaseSensitive NOTIFY caseSensitiveChanged)
    Q_PROPERTY(bool minimal READ minimal WRITE setMinimal NOTIFY minimalChanged)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(int matchIndex READ matchIndex NOTIFY regexpChanged)
    Q_PROPERTY(int matchLength READ matchLength NOTIFY regexpChanged)
    Q_PROPERTY(bool capture0Available READ capture0Available NOTIFY regexpChanged)
    Q_PROPERTY(QString capture0Text READ capture0Text NOTIFY regexpChanged)
    Q_PROPERTY(bool capture1Available READ capture1Available NOTIFY regexpChanged)
    Q_PROPERTY(QString capture1Text READ capture1Text NOTIFY regexpChanged)
    Q_PROPERTY(bool capture2Available READ capture2Available NOTIFY regexpChanged)
    Q_PROPERTY(QString capture2Text READ capture2Text NOTIFY regexpChanged)
    Q_PROPERTY(bool capture3Available READ capture3Available NOTIFY regexpChanged)
    Q_PROPERTY(QString capture3Text READ capture3Text NOTIFY regexpChanged)
    Q_PROPERTY(bool capture4Available READ capture4Available NOTIFY regexpChanged)
    Q_PROPERTY(QString capture4Text READ capture4Text NOTIFY regexpChanged)
    Q_PROPERTY(bool capture5Available READ capture5Available NOTIFY regexpChanged)
    Q_PROPERTY(QString capture5Text READ capture5Text NOTIFY regexpChanged)

public:
    RegExp(QObject *parent = 0);

    // The accessor methods for the properties
    void setPattern(const QString &pattern);
    QString pattern() const;

    void setPatternSyntax(QRegExp::PatternSyntax syntax);

    QString escapedPattern() const;

    void setCaseSensitive(bool caseSensitive);
    bool caseSensitive() const;

    void setMinimal(bool minimal);
    bool minimal() const;

    void setText(const QString &text);
    QString text() const;

    int matchIndex() const;

    int matchLength() const;

    bool capture0Available() const;
    QString capture0Text() const;

    bool capture1Available() const;
    QString capture1Text() const;

    bool capture2Available() const;
    QString capture2Text() const;

    bool capture3Available() const;
    QString capture3Text() const;

    bool capture4Available() const;
    QString capture4Text() const;

    bool capture5Available() const;
    QString capture5Text() const;

Q_SIGNALS:
    // The change notification methods for the properties
    void patternChanged();
    void caseSensitiveChanged();
    void minimalChanged();
    void textChanged();
    void regexpChanged();

private:
    /**
     * A helper method that starts the processing of the regular expression whenever
     * an input value has been changed.
     */
    void refresh();

    /**
     * A helper structure to store the availability and actual text of a capture.
     */
    struct Capture
    {
        bool available;
        QString text;
    };

    // The property values
    QString m_pattern;
    QString m_escapedPattern;
    QRegExp::PatternSyntax m_patternSyntax;
    bool m_caseSensitive;
    bool m_minimal;
    QString m_text;
    int m_matchIndex;
    int m_matchLength;
    int m_matchText;

    // The list of captures for the current regular expression
    QVector<Capture> m_captures;
};

#endif
