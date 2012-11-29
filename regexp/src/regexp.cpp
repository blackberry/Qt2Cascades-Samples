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

#include "regexp.hpp"

static const int s_capturesCount = 6;

//! [0]
RegExp::RegExp(QObject *parent)
    : QObject(parent)
    , m_pattern("[A-Za-z_]+([A-Za-z_0-9]*)")
    , m_patternSyntax(QRegExp::RegExp)
    , m_caseSensitive(true)
    , m_minimal(false)
    , m_text("(10 + delta4) * 32")
    , m_matchIndex(-1)
    , m_matchLength(-1)
{
    // Initialize the list of captures with 'no captures available'
    for (int i = 0; i < s_capturesCount; ++i) {
        Capture capture;
        capture.available = false;

        m_captures << capture;
    }

    // Start initial processing
    refresh();
}
//! [0]

void RegExp::setPattern(const QString &pattern)
{
    if (m_pattern == pattern)
        return;

    m_pattern = pattern;
    refresh();

    emit patternChanged();
}

QString RegExp::pattern() const
{
    return m_pattern;
}

void RegExp::setPatternSyntax(QRegExp::PatternSyntax syntax)
{
    if (m_patternSyntax == syntax)
        return;

    m_patternSyntax = syntax;
    refresh();
}

QString RegExp::escapedPattern() const
{
    return m_escapedPattern;
}

void RegExp::setCaseSensitive(bool caseSensitive)
{
    if (m_caseSensitive == caseSensitive)
        return;

    m_caseSensitive = caseSensitive;
    refresh();

    emit caseSensitiveChanged();
}

bool RegExp::caseSensitive() const
{
    return m_caseSensitive;
}

void RegExp::setMinimal(bool minimal)
{
    if (m_minimal == minimal)
        return;

    m_minimal = minimal;
    refresh();

    emit minimalChanged();
}

bool RegExp::minimal() const
{
    return m_minimal;
}

//! [1]
void RegExp::setText(const QString &text)
{
    if (m_text == text)
        return;

    m_text = text;
    refresh();

    emit textChanged();
}
//! [1]

QString RegExp::text() const
{
    return m_text;
}

int RegExp::matchIndex() const
{
    return m_matchIndex;
}

int RegExp::matchLength() const
{
    return m_matchLength;
}

//! [2]
bool RegExp::capture0Available() const
{
    return m_captures.at(0).available;
}

QString RegExp::capture0Text() const
{
    return m_captures.at(0).text;
}
//! [2]

bool RegExp::capture1Available() const
{
    return m_captures.at(1).available;
}

QString RegExp::capture1Text() const
{
    return m_captures.at(1).text;
}

bool RegExp::capture2Available() const
{
    return m_captures.at(2).available;
}

QString RegExp::capture2Text() const
{
    return m_captures.at(2).text;
}

bool RegExp::capture3Available() const
{
    return m_captures.at(3).available;
}

QString RegExp::capture3Text() const
{
    return m_captures.at(3).text;
}

bool RegExp::capture4Available() const
{
    return m_captures.at(4).available;
}

QString RegExp::capture4Text() const
{
    return m_captures.at(4).text;
}

bool RegExp::capture5Available() const
{
    return m_captures.at(5).available;
}

QString RegExp::capture5Text() const
{
    return m_captures.at(5).text;
}

//! [3]
void RegExp::refresh()
{
    // Create the escaped version of the pattern string ...
    QString escaped = m_pattern;
    escaped.replace("\\", "\\\\");
    escaped.replace("\"", "\\\"");
    escaped.prepend("\"");
    escaped.append("\"");

    // ... and store it.
    m_escapedPattern = escaped;

    // Create a temporary QRegExp object with the current pattern as parameter
    QRegExp regexp(m_pattern);

    // Configure the QRegExp object according to the other input parameters
    regexp.setCaseSensitivity(m_caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive);
    regexp.setMinimal(m_minimal);
    regexp.setPatternSyntax(m_patternSyntax);

    // Do the actual pattern matching against the input string (m_text) ...
    m_matchIndex = regexp.indexIn(m_text);

    // ... and fill the output parameters with the new values.
    m_matchLength = regexp.matchedLength();

    for (int i = 0; i < s_capturesCount; ++i) {
        m_captures[i].available = (i <= regexp.captureCount());
        m_captures[i].text = regexp.cap(i);
    }

    // Notify the UI that the output parameters have changed
    emit regexpChanged();
}
//! [3]
