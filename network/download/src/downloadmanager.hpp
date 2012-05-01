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

#ifndef DOWNLOADMANAGER_HPP
#define DOWNLOADMANAGER_HPP

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtNetwork/QNetworkAccessManager>

class QNetworkReply;
class QUrl;

/**
 * The DownloadManager encapsulates the download and saving of URLs.
 * Error and status messages are available to the UI via properties as well as the number
 * of currently running downloads.
 */
class DownloadManager : public QObject
{
    Q_OBJECT

    // Makes error messages available to the UI
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)

    // Makes status messages available to the UI
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)

    // Makes the number of currently running downloads available to the UI
    Q_PROPERTY(int activeDownloads READ activeDownloads NOTIFY activeDownloadsChanged)

public:
    DownloadManager();

    // The accessor methods for the properties
    QString errorMessage() const;
    QString statusMessage() const;
    int activeDownloads() const;

public Q_SLOTS:
    // This method is called when the user starts a download by clicking the 'Download' button in the UI
    void downloadUrl(const QString &url);

Q_SIGNALS:
    // The change notification signals of the properties
    void errorMessageChanged();
    void statusMessageChanged();
    void activeDownloadsChanged();

private Q_SLOTS:
    // This method is called when a download has been finished
    void downloadFinished(QNetworkReply *reply);

    // This method is called when an SSL-related error occurrs during connection establishment
    void sslErrors(const QList<QSslError> &errors);

private:
    // This method starts the download for the given URL
    void doDownload(const QUrl &url);

    // This method determines a file name that can be used to save the given URL
    QString saveFileName(const QUrl &url);

    // This method saves the received data under the given file name
    bool saveToDisk(const QString &filename, QIODevice *data);

    // The network access manager that does all the network communication
    QNetworkAccessManager m_manager;

    // A list of currently running downloads
    QList<QNetworkReply *> m_currentDownloads;

    // A list of error messages
    QStringList m_errorMessage;

    // A list of status messages
    QStringList m_statusMessage;
};

#endif
