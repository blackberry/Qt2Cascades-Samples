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

#include <QtNetwork/QNetworkRequest>

//! [1]
#include "googlesuggest.hpp"

using namespace bb::cascades;

const char *GSUGGEST_URL = "http://google.com/complete/search?output=toolbar&q=%1";
//! [1]

//! [2]
GoogleSuggest::GoogleSuggest()
{
    // We use a timer that goes off after 500ms to trigger
    // actually retrieving the suggestions
    m_timer.setSingleShot(true);
    m_timer.setInterval(500);

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(autoSuggest()));

    // Hook this signal so we get our network requests
    connect(&m_networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(handleNetworkData(QNetworkReply*)));
}
//! [2]

QString GoogleSuggest::input() const
{
    return m_input;
}

void GoogleSuggest::setInput(const QString &input)
{
    if (input == m_input)
        return;

    // We need to save this to access later once we want
    // to retrieve the suggestions
    m_input = input;
    m_timer.start();
}

bb::cascades::DataModel* GoogleSuggest::model() const
{
    return &m_model;
}

//! [3]
void GoogleSuggest::showCompletions(const QStringList &choices)
{
    emit clearSelection();

    m_model.clear();

    if (choices.isEmpty())
        return;

    for (int i = 0; i < choices.count(); ++i) {
        QVariantMap item;

        item["suggestion"] = QString(choices[i]);
        m_model.append(item);
    }
}
//! [3]

//! [4]
void GoogleSuggest::autoSuggest()
{
    if (m_input.isEmpty()) {
        m_model.clear();
        return;
    }

    const QString url = QString::fromLatin1(GSUGGEST_URL).arg(m_input);

    m_networkManager.get(QNetworkRequest(url));
}
//! [4]

//! [5]
void GoogleSuggest::handleNetworkData(QNetworkReply *networkReply)
{
    if (!networkReply->error()) {
        QStringList choices;

        const QByteArray response(networkReply->readAll());
        QXmlStreamReader xml(response);
        while (!xml.atEnd()) {
            xml.readNext();
            if (xml.tokenType() == QXmlStreamReader::StartElement) {
                if (xml.name() == "suggestion") {
                    choices << xml.attributes().value("data").toString();
                }
            }
        }

        showCompletions(choices);
    }

    networkReply->deleteLater();
}
//! [5]
