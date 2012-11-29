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

#ifndef RSSLISTING_HPP
#define RSSLISTING_HPP

#include <QtCore/QBuffer>
#include <QtCore/QUrl>
#include <QtCore/QXmlStreamReader>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

/**
 * The RSSListing class encapsulates the download and parsing of
 * the RSS feed. The feed URL and the extracted articles are available
 * to the UI via properties.
 */
class RSSListing : public QObject
{
    Q_OBJECT

    // The URL where the RSS feed is downloaded from
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)

    // Makes the extracted articles available to the UI
    Q_PROPERTY(QString feeds READ feeds NOTIFY feedsChanged)

    // Makes the state of the RSSListing available to the UI
    Q_PROPERTY(bool active READ active NOTIFY activeChanged)

public:
    RSSListing(QObject *object = 0);

public Q_SLOTS:
    // This method is invoked from the UI to start the download of the RSS feed
    void fetch();

Q_SIGNALS:
    // The change notification signals of the properties
    void urlChanged();
    void feedsChanged();
    void activeChanged();

private Q_SLOTS:
    // This method is called when the feed has been downloaded
    void finished(QNetworkReply *reply);

    // This method is called whenever new data from the feed have been received
    void readyRead();

    // This method is called when the meta data of the HTTP connection have changed (e.g. the server sent a redirect response)
    void metaDataChanged();

    // This method is called if an network related error occurred
    void error(QNetworkReply::NetworkError);

private:
    // The accessor methods of the properties
    void setUrl(const QString &url);
    QString url() const;

    QString feeds() const;

    bool active() const;

    // This method parses the RSS feed after it has been downloaded
    void parseXml();

    // This method triggers the actual download of the RSS feed
    void get(const QUrl &url);

    // The property values
    QString m_url;
    QString m_feeds;
    bool m_active;

    // The XML stream reader that is used to extract the articles from the RSS feed
    QXmlStreamReader m_xml;

    // The network manager that is used to download the RSS feed
    QNetworkAccessManager m_manager;

    // The network reply that represents the current download job
    QNetworkReply *m_currentReply;
};

#endif

