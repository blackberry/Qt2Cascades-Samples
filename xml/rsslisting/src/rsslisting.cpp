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

/*
 rsslisting.cpp

 Provides the business logic for retrieving news items from RDF news sources.
 RDF is an XML-based format for storing items of information (see
 http://www.w3.org/RDF/ for details).

 The class downloads and parses the XML asynchronously, feeding the
 data to an XML reader in pieces. This allows the user to interrupt
 its operation, and also allows very large data sources to be read.
 */

#include "rsslisting.hpp"

/*
 Constructs an RSSListing object and connects the needed signals.
 */
//! [0]
RSSListing::RSSListing(QObject *parent)
    : QObject(parent)
    , m_url("http://feeds.feedburner.com/blackberry/CAxx")
    , m_active(false)
    , m_currentReply(0)
{
    // Invoke the finished() method whenever the download job has been finished
    connect(&m_manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(finished(QNetworkReply*)));
}
//! [0]

/*
 Starts the network request and connects the needed signals
 */
//! [1]
void RSSListing::get(const QUrl &url)
{
    QNetworkRequest request(url);
    if (m_currentReply) {
        m_currentReply->disconnect(this);
        m_currentReply->deleteLater();
    }

    m_currentReply = m_manager.get(request);
    connect(m_currentReply, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(m_currentReply, SIGNAL(metaDataChanged()), this, SLOT(metaDataChanged()));
    connect(m_currentReply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(error(QNetworkReply::NetworkError)));
}
//! [1]

/*
 Starts fetching data from a news source specified in m_url.

 The UI is made read only to prevent the user from modifying its
 contents during the fetch; this is only for cosmetic purposes.

 A URL is created with the raw contents of m_url and
 a get is initiated.
 */

void RSSListing::fetch()
{
    m_active = true;
    emit activeChanged();

    m_xml.clear();

    const QUrl url(m_url);
    get(url);
}

/*
 If we got a redirect response from the server, trigger a new download
 with the redirect URL.
 */
void RSSListing::metaDataChanged()
{
    const QUrl redirectionTarget = m_currentReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if (redirectionTarget.isValid()) {
        get(redirectionTarget);
    }
}

/*
 Reads data received from the RDF source.

 We read all the available data, and pass it to the XML
 stream reader.
 */

//! [2]
void RSSListing::readyRead()
{
    const int statusCode = m_currentReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode >= 200 && statusCode < 300) {
        const QByteArray data = m_currentReply->readAll();
        m_xml.addData(data);
    }
}
//! [2]

/*
 Finishes processing an HTTP request.

 If an error has occurred, the user interface is made available
 to the user for further input, allowing a new fetch to be
 started.

 If the HTTP get request has finished, we make the
 user interface available to the user for further input.
 */

void RSSListing::finished(QNetworkReply *reply)
{
    Q_UNUSED(reply);

    parseXml();

    m_active = false;
    emit activeChanged();
}

void RSSListing::setUrl(const QString &url)
{
    if (m_url == url)
        return;

    m_url = url;
    emit urlChanged();
}

QString RSSListing::url() const
{
    return m_url;
}

QString RSSListing::feeds() const
{
    return m_feeds;
}

bool RSSListing::active() const
{
    return m_active;
}

/*
 Parses the XML data and fill feeds string accordingly.
 */
//! [3]
void RSSListing::parseXml()
{
    m_feeds.clear();

    QString currentTag;
    QString linkString;
    QString titleString;

    while (!m_xml.atEnd()) {
        m_xml.readNext();
        if (m_xml.isStartElement()) {
            if (m_xml.name() == "item")
                linkString = m_xml.attributes().value("rss:about").toString();
            currentTag = m_xml.name().toString();
        } else if (m_xml.isEndElement()) {
            if (m_xml.name() == "item") {

                m_feeds.append(QString::fromLatin1("%1\n      %2\n\n").arg(titleString, linkString));

                titleString.clear();
                linkString.clear();
            }

        } else if (m_xml.isCharacters() && !m_xml.isWhitespace()) {
            if (currentTag == "title")
                titleString += m_xml.text().toString();
            else if (currentTag == "link")
                linkString += m_xml.text().toString();
        }
    }

    if (m_xml.error() && m_xml.error() != QXmlStreamReader::PrematureEndOfDocumentError) {
        m_feeds.append(
                QString::fromLatin1("XML ERROR: %1: %2").arg(m_xml.lineNumber()).arg(m_xml.errorString()));
    }

    emit feedsChanged();
}
//! [3]

void RSSListing::error(QNetworkReply::NetworkError)
{
    m_feeds = QLatin1String("error retrieving RSS feed");
    emit feedsChanged();

    m_currentReply->disconnect(this);
    m_currentReply->deleteLater();
    m_currentReply = 0;
}
