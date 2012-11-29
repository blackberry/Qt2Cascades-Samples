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

#ifndef HTTPDOWNLOADER_HPP
#define HTTPDOWNLOADER_HPP

#include "AuthenticationDialogController.hpp"
#include "MessageBoxController.hpp"

#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>

class QFile;

//! [1]
/**
 * The HttpDownloader class contains all the logic to download URLs from
 * the network and store them in a local file.
 */
class HttpDownloader : public QObject
{
    Q_OBJECT

    // Makes available to the UI whether the next download can be started
    Q_PROPERTY(bool startDownloadPossible READ startDownloadPossible NOTIFY startDownloadPossibleChanged)

    // Makes the status text available to the UI
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)
//! [1]
public:
    explicit HttpDownloader(QObject *parent = 0);
    ~HttpDownloader();
//! [2]
    // This method is executed when the user clicks the 'Download' button in the UI
    Q_INVOKABLE void startDownload(const QString &url);
//! [2]

    // The accessor methods for the properties
    bool startDownloadPossible() const;
    QString statusText() const;
//! [3]
    // The accessor methods for the dialog controllers
    MessageBoxController *messageBoxController() const;
    AuthenticationDialogController *authenticationDialogController() const;
//! [3]

Q_SIGNALS:
    // The change notification signals of the properties
    void startDownloadPossibleChanged();
    void statusTextChanged();
//! [5]
private Q_SLOTS:
    // This method is called whenever a download job has finished
    void httpFinished();

    // This method is called whenever a download job received new data
    void httpReadyRead();

    // This method is called if authentication is required to continue the download
    void slotAuthenticationRequired(QNetworkReply*, QAuthenticator *authenticator);

    // This method is called if a error occurred during the initial SSL handshake
    void sslErrors(QNetworkReply*, const QList<QSslError> &errors);
//! [5]

private:
    // A helper method to change the 'startDownloadPossible' property
    void setStartDownloadPossible(bool possible);

    // Starts a new download job
    void startRequest();

    // A flag to mark whether a new download can be started
    bool m_startDownloadPossible;

    // The status text for the current download
    QString m_statusText;

    // The source URL of the current download
    QUrl m_url;

    // The target file of the current download
    QFile *m_file;
//! [6]
    // The network access manager
    QNetworkAccessManager m_qnam;
//! [6]
//! [7]
    // The network reply that represents the current download
    QNetworkReply *m_reply;
//! [7]

    // The file name of the target file
    QString m_fileName;

    // The dialog controllers
    MessageBoxController m_messageBoxController;
    AuthenticationDialogController m_authenticationDialogController;
};

#endif
