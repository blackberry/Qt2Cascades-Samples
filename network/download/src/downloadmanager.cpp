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

#include "downloadmanager.hpp"

#include <QCoreApplication>
#include <QFile>
#include <QFileInfo>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslError>
#include <QStringList>
#include <QUrl>

#include <stdio.h>

QT_BEGIN_NAMESPACE
class QSslError;
QT_END_NAMESPACE

QT_USE_NAMESPACE

DownloadManager::DownloadManager()
{
    /**
     * Create signal/slot connection so that whenever the network access manager
     * signals the completion of a download, downloadFinished() is invoked.
     */
    connect(&m_manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(downloadFinished(QNetworkReply*)));
}

QString DownloadManager::errorMessage() const
{
    return m_errorMessage.join("\n");
}

QString DownloadManager::statusMessage() const
{
    return m_statusMessage.join("\n");
}

int DownloadManager::activeDownloads() const
{
    return m_currentDownloads.count();
}

void DownloadManager::downloadUrl(const QString &url)
{
    // We just forward this call to doDownload() and convert the parameter
    doDownload(QUrl(url));
}

void DownloadManager::doDownload(const QUrl &url)
{
    // This method starts the actual download

    // First create a network request ...
    QNetworkRequest request(url);

    // ... and then pass it to the network access manager.
    QNetworkReply *reply = m_manager.get(request);

    /**
     * The returned network reply is a handle that can be used to monitor
     * the progress of the download, receive error messages or abort the download.
     *
     * Note: We have the ownership of the reply object, that means we have to delete it
     *       explicitly when no longer needed.
     */

    // Invoke sslErrors() whenever SSL-related errors occurr during connection establishment
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
            SLOT(sslErrors(QList<QSslError>)));

    // Store the reply in our list of running downloads
    m_currentDownloads.append(reply);

    emit activeDownloadsChanged();
}

QString DownloadManager::saveFileName(const QUrl &url)
{
    // First extract the path component from the URL ...
    const QString path = url.path();

    // ... and then extract the file name.
    QString basename = QFileInfo(path).fileName();

    // Replace the file name with 'download' if the URL provides no file name.
    if (basename.isEmpty())
        basename = "download";

    basename = "tmp/" + basename; // locate in tmp directory

    /**
     * Check if the file name exists already, if so, append an increasing number and test again.
     */
    if (QFile::exists(basename)) {
        // already exists, don't overwrite
        int i = 0;
        basename += '.';
        while (QFile::exists(basename + QString::number(i)))
            ++i;

        basename += QString::number(i);
    }

    return basename;
}

bool DownloadManager::saveToDisk(const QString &filename, QIODevice *data)
{
    // Open the file in write mode
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        m_errorMessage << QString("Could not open %1 for writing: %2").arg(filename).arg(file.errorString());
        emit errorMessageChanged();
        return false;
    }

    // Write all received data to the file
    file.write(data->readAll());
    file.close();

    return true;
}

void DownloadManager::sslErrors(const QList<QSslError> &sslErrors)
{
#ifndef QT_NO_OPENSSL
    foreach (const QSslError &error, sslErrors) {
        m_errorMessage << QString("SSL error: %1").arg(error.errorString());
        emit errorMessageChanged();
    }
#endif
}

void DownloadManager::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();

    // Check whether the download was aborted by an error
    if (reply->error()) {
        m_errorMessage << QString("Download of %1 failed: %2").arg(url.toString()).arg(reply->errorString());
        emit errorMessageChanged();
    } else {
        // If the download was successful, get a unique file name ...
        QString filename = saveFileName(url);

        // ... and save the received data to that file.
        if (saveToDisk(filename, reply)) {
            m_statusMessage << QString("Download of %1 succeeded (saved to %2)").arg(url.toString()).arg(filename);
            emit statusMessageChanged();
        }
    }

    // The download is finished, so remove it from the list of running downloads
    m_currentDownloads.removeAll(reply);
    emit activeDownloadsChanged();

    /**
     * We can't call 'delete reply' here, because this method might have been invoked directly as result of a signal
     * emission of the reply object.
     */
    reply->deleteLater();
}
