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

#include "HttpDownloader.hpp"

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtNetwork/QAuthenticator>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QSslError>

HttpDownloader::HttpDownloader(QObject *parent)
    : QObject(parent), m_startDownloadPossible(true), m_statusText(tr("Please enter the URL of a file you want to download.")), m_file(0), m_reply(0)
{
    // Create signal/slot connections so that we can react to authentication requests and SSL-related errors.
    connect(&m_qnam, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),
            this, SLOT(slotAuthenticationRequired(QNetworkReply*,QAuthenticator*)));
    connect(&m_qnam, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
            this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
}

HttpDownloader::~HttpDownloader()
{
    delete m_file;
    delete m_reply;
}

bool HttpDownloader::startDownloadPossible() const
{
    return m_startDownloadPossible;
}

void HttpDownloader::setStartDownloadPossible(bool possible)
{
    if (possible != m_startDownloadPossible) {
        m_startDownloadPossible = possible;
        emit startDownloadPossibleChanged();
    }
}

QString HttpDownloader::statusText() const
{
    return m_statusText;
}

MessageBoxController* HttpDownloader::messageBoxController() const
{
    return const_cast<MessageBoxController*>(&m_messageBoxController);
}

AuthenticationDialogController *HttpDownloader::authenticationDialogController() const
{
    return const_cast<AuthenticationDialogController*>(&m_authenticationDialogController);
}

//! [1]
void HttpDownloader::startDownload(const QString &url)
{
//! [2]
    m_url = url;

    // Extract the file name from the URL ...
    const QFileInfo fileInfo(m_url.path());
    m_fileName = fileInfo.fileName();

    // ... and fall back to 'index.html' if it is empty.
    if (m_fileName.isEmpty())
        m_fileName = "index.html";
//! [2]
//! [3]
    // Locate the file in the temp directory
    const QString actualFileName = "tmp/" + m_fileName;

    // If the file already exists, ask whether it should be overwritten
    if (QFile::exists(actualFileName)) {
        const MessageBoxController::Result result = m_messageBoxController.exec(tr("HTTP"),
                                                                                tr("There already exists a file called %1 in "
                                                                                   "the target directory.\nOverwrite?").arg(m_fileName),
                                                                                tr("Overwrite"), tr("Cancel"));
        if (result == MessageBoxController::Button2) {
            // If it shouldn't be overwritten, abort here ...
            return;
        }

        // ... otherwise remove the existing file.
        QFile::remove(actualFileName);
    }
//! [3]
//! [4]
    // Open the file for writing
    m_file = new QFile(actualFileName);
    if (!m_file->open(QIODevice::WriteOnly)) {
        m_messageBoxController.exec(tr("HTTP"), tr("Unable to save the file %1: %2.").arg(m_fileName).arg(m_file->errorString()), tr("Ok"), QString());
        delete m_file;
        m_file = 0;
        return;
    }

    setStartDownloadPossible(false);

    // Trigger the download
    startRequest();
//! [4]
}
//! [1]
//! [5]
void HttpDownloader::startRequest()
{
    // Start the download ...
    m_reply = m_qnam.get(QNetworkRequest(m_url));

    // ... and create signal/slot connections to get informed about state changes
    connect(m_reply, SIGNAL(finished()), this, SLOT(httpFinished()));
    connect(m_reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
}
//! [5]
//! [6]
void HttpDownloader::httpFinished()
{
    // All data have been written to the file, so close it
    m_file->flush();
    m_file->close();
    const QVariant redirectionTarget = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    if (m_reply->error()) {
        // If there was an error, show an error message in a message box
        m_messageBoxController.exec(tr("HTTP"), tr("Download failed: %1.").arg(m_reply->errorString()), tr("Ok"), QString());

        setStartDownloadPossible(true);
    } else if (!redirectionTarget.isNull()) {
        // If we got a redirect response (3XX), retrieve the redirect URL ...
        const QUrl newUrl = m_url.resolved(redirectionTarget.toUrl());

        // ... and ask the user whether the redirected page should be downloaded instead
        const MessageBoxController::Result result = m_messageBoxController.exec(tr("HTTP"), tr("Redirect to %1 ?").arg(newUrl.toString()), tr("Yes"), tr("No"));

        if (result == MessageBoxController::Button1) {
            // If the redirected page should be downloaded, reset the URL ...
            m_url = newUrl;

            // ... delete the old network reply object ...
            m_reply->deleteLater();

            // ... reset the target file ...
            m_file->open(QIODevice::WriteOnly);
            m_file->resize(0);

            // ... and trigger a new download request with the new URL.
            startRequest();
            return;
        } else {
            setStartDownloadPossible(true);
        }
    } else {
        // If the download was successful, update the status message
        const QFileInfo actualDir(*m_file);
        m_statusText = tr("Downloaded %1 to %2.").arg(m_fileName).arg(actualDir.absolutePath());
        emit statusTextChanged();
        setStartDownloadPossible(true);
    }

    // Delete the network reply object
    m_reply->deleteLater();
    m_reply = 0;

    // Clean up the target file object
    delete m_file;
    m_file = 0;
}
//! [6]

void HttpDownloader::httpReadyRead()
{
    /**
     * This slot gets called every time the QNetworkReply has new data.
     * We read all of its new data and write it into the file.
     * That way we use less RAM than when reading it at the finished()
     * signal of the QNetworkReply
     */
    if (m_file) {
        m_file->write(m_reply->readAll());
    }
}

void HttpDownloader::slotAuthenticationRequired(QNetworkReply*, QAuthenticator *authenticator)
{
    /**
     * This method is called by the QNetworkAccessManager if an authentication is required.
     *
     * In this case we bring up a password dialog to ask the user for user name and password.
     * If the user entered data and clicked 'Ok' (exec() returns 'true'), we set the data on the
     * QAuthenticator object and the QNetworkAccessManager will try to re-authenticate at the server
     * with the new credentials. Otherwise the download will be aborted.
     */

    const QString siteString = tr("%1 at %2").arg(authenticator->realm()).arg(m_url.host());
    if (m_authenticationDialogController.exec(siteString, m_url.userName(), m_url.password())) {
        authenticator->setUser(m_authenticationDialogController.user());
        authenticator->setPassword(m_authenticationDialogController.password());
    }
}

void HttpDownloader::sslErrors(QNetworkReply*, const QList<QSslError> &errors)
{
    /**
     * This method is called by the QNetworkAccessManager if an SSL-related error occurred
     * (e.g. validity of server certificates couldn't be verified).
     *
     * In this case we bring up a message box to ask the user to ignore the errors or abort the
     * download. If the errors should be ignored (exec() returns Button1) we call ignoreSslErrors()
     * to tell the QNetworkAccessManager that it should ignore this error.
     */
    QString errorString;
    foreach (const QSslError &error, errors) {
        if (!errorString.isEmpty())
            errorString += "\n";
        errorString += "  " + error.errorString();
    }

    const MessageBoxController::Result result = m_messageBoxController.exec(tr("HTTP"),
                                                                            tr("One or more SSL errors have occurred:\n%1").arg(errorString),
                                                                            tr("Ignore"), tr("Abort"));
    if (result == MessageBoxController::Button1) {
        m_reply->ignoreSslErrors();
    }
}
