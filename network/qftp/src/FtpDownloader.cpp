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

#include "FtpDownloader.hpp"

#include <bb/cascades/Button>
#include <bb/cascades/Label>
#include <bb/cascades/ListView>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/TextField>

#include <QtCore/QFile>

using namespace bb::cascades;

FtpDownloader::FtpDownloader(QObject *parent)
    : QObject(parent)
    , m_url(QLatin1String("ftp://ftp.kde.org"))
    , m_ftp(0)
    , m_file(0)
    , m_parentDirectoryAvailable(false)
    , m_downloadPossible(false)
    , m_connectPossible(true)
    , m_connectLabel(tr("Connect"))
    , m_selectionPossible(false)
    , m_networkSession(0)
{
    // Initialize the properties with some default values to bring the UI in a sane state on startup

    m_statusText = tr("Please enter the name of an FTP server.");
    emit statusTextChanged();

    m_selectionPossible = false;
    emit selectionPossibleChanged();

    m_parentDirectoryAvailable = false;
    emit parentDirectoryAvailableChanged();

    m_downloadPossible = false;
    emit downloadPossibleChanged();

    // Call the cancelDownload() method whenever the user presses the 'Cancel' button in the download progress dialog
    connect(&m_progressDialogController, SIGNAL(canceled()), this, SLOT(cancelDownload()));
}

FtpDownloader::~FtpDownloader()
{
    delete m_file;
    m_file = 0;
}

//![0]
void FtpDownloader::connectOrDisconnect()
{
    // This method is called whenever the user clicks on the 'Connect/Disconnect' button in the UI

    // If we are currently connected (we have a valid FTP client object), we'll disconnect
    if (m_ftp) {
        // Cleanup the FTP client object
        m_ftp->abort();
        m_ftp->deleteLater();
        m_ftp = 0;
//![0]
        // Reset all properties to a disconnected state
        m_selectionPossible = false;
        emit selectionPossibleChanged();

        m_parentDirectoryAvailable = false;
        emit parentDirectoryAvailableChanged();

        m_downloadPossible = false;
        emit downloadPossibleChanged();

        m_connectPossible = true;
        emit connectPossibleChanged();

        m_connectLabel = tr("Connect");
        emit connectLabelChanged();

        m_statusText = tr("Please enter the name of an FTP server.");
        emit statusTextChanged();
        return;
    }

    // Before we try to create a new FTP client object, we check the availability of a network connection

    if (!m_networkSession || !m_networkSession->isOpen()) { // If we have no network session available yet...

        // ... check if the platform requires that a network session is created before network operations can be performed
        if (m_manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {

            if (!m_networkSession) {
                // Get saved network configuration
                QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
                settings.beginGroup(QLatin1String("QtNetwork"));
                const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
                settings.endGroup();

                // If the saved network configuration is not currently discovered use the system default
                QNetworkConfiguration config = m_manager.configurationFromIdentifier(id);
                if ((config.state() & QNetworkConfiguration::Discovered) != QNetworkConfiguration::Discovered) {
                    config = m_manager.defaultConfiguration();
                }

                m_networkSession = new QNetworkSession(config, this);

                // As soon as the network session is established, try to connect to the FTP server
                connect(m_networkSession, SIGNAL(opened()), this, SLOT(connectToFtp()));

                connect(m_networkSession, SIGNAL(error(QNetworkSession::SessionError)),
                        this, SLOT(enableConnectButton()));
            }

            m_connectPossible = false;
            emit connectPossibleChanged();

            m_statusText = tr("Opening network session.");
            emit statusTextChanged();

            // Trigger to open the network session
            m_networkSession->open();
            return;
        }
    }

    // ... otherwise if a we have an open network session, try to connect to the FTP server immediately
    connectToFtp();
}

void FtpDownloader::setUrl(const QString &url)
{
    if (m_url != url) {
        m_url = url;
        emit urlChanged();
    }
}

QString FtpDownloader::url() const
{
    return m_url;
}

QString FtpDownloader::statusText() const
{
    return m_statusText;
}

bool FtpDownloader::parentDirectoryAvailable() const
{
    return m_parentDirectoryAvailable;
}

bool FtpDownloader::downloadPossible() const
{
    return m_downloadPossible;
}

bool FtpDownloader::connectPossible() const
{
    return m_connectPossible;
}

QString FtpDownloader::connectLabel() const
{
    return m_connectLabel;
}

bool FtpDownloader::selectionPossible() const
{
    return m_selectionPossible;
}

void FtpDownloader::connectToFtp()
{
//![1]
    // Create a new FTP client object and connect to the signals for receiving status changes
    m_ftp = new QFtp(this);
    connect(m_ftp, SIGNAL(commandFinished(int,bool)),
            this, SLOT(ftpCommandFinished(int,bool)));
    connect(m_ftp, SIGNAL(listInfo(QUrlInfo)),
            this, SLOT(addToList(QUrlInfo)));
    connect(m_ftp, SIGNAL(dataTransferProgress(qint64,qint64)),
            this, SLOT(updateDataTransferProgress(qint64,qint64)));

    // Reset the model and current path before we start navigation on the FTP server
    m_model.clear();
    m_currentPath.clear();
//![1]

//![2]
    // Connect to the FTP server (with credentials if available) and navigate to the given path
    const QUrl url(m_url);
    if (!url.isValid() || url.scheme().toLower() != QLatin1String("ftp")) {
        m_ftp->connectToHost(m_url, 21);
        m_ftp->login();
    } else {
        m_ftp->connectToHost(url.host(), url.port(21));

        if (!url.userName().isEmpty())
            m_ftp->login(QUrl::fromPercentEncoding(url.userName().toLatin1()), url.password());
        else
            m_ftp->login();
        if (!url.path().isEmpty()) {
            m_currentPath = url.path();
            if (m_currentPath.endsWith('/'))
                m_currentPath.chop(1);

            m_ftp->cd(m_currentPath);
        }
    }
//![2]

    // Update the status properties to reflect that we can start navigation on the FTP server
    m_selectionPossible = true;
    emit selectionPossibleChanged();

    m_connectPossible = false;
    emit connectPossibleChanged();

    m_connectLabel = tr("Disconnect");
    emit connectLabelChanged();

    m_statusText = tr("Connecting to FTP server %1...").arg(m_url);
    emit statusTextChanged();
}

//![3]
void FtpDownloader::downloadFile()
{
    // Retrieve the selected item from the directory listing ListView
    const QVariant data = m_model.data(m_currentIndexPath);
    if (!data.isValid()) {
        enableDownloadButton();
        return;
    }

    const FtpItem currentItem = data.value<FtpItem>();

    // Assemble the target file name for the download
    m_downloadFileName = currentItem.fileName;
    const QString diskFileName = "tmp/" + m_downloadFileName;
//![3]
//
    // If the target file name exists already, report an error
    if (QFile::exists(diskFileName)) {
        m_messageBoxController.exec(tr("FTP"),
                tr("There already exists a file called %1 in %2.").arg(diskFileName).arg("tmp/"),
                tr("Ok"), QString());
        return;
    }

//![4]
    // Try to open the target file and report an error if that's not possible
    m_file = new QFile(diskFileName);
    if (!m_file->open(QIODevice::WriteOnly)) {
        m_messageBoxController.exec(tr("FTP"),
                tr("Unable to save the file %1: %2.").arg(diskFileName).arg(m_file->errorString()),
                tr("Ok"), QString());
        delete m_file;
        m_file = 0;
        return;
    }

    // Start the download of the selected item on the FTP server to the local target file
    m_ftp->get(m_downloadFileName, m_file);

    // Update the status properties to reflect the started download
    m_progressDialogController.setLabelText(tr("Downloading %1...").arg(m_downloadFileName));
    m_downloadPossible = false;
    emit downloadPossibleChanged();

    // Show the progress dialog
    m_progressDialogController.show();
}
//![4]

//![5]
void FtpDownloader::cancelDownload()
{
    // If the user wants to cancel the download, tell the FTP client about it ...
    m_ftp->abort();

    // ... and clean up the local target file
    if (m_file->exists()) {
        m_file->close();
        m_file->remove();
    }
    delete m_file;
    m_file = 0;
}
//![5]

//![6]
void FtpDownloader::ftpCommandFinished(int, bool error)
{
    // This method is called whenever a FTP command has been processed by the FTP client

    if (m_ftp->currentCommand() == QFtp::ConnectToHost) {
        // If the connect failed, show an error dialog ...
        if (error) {
            m_messageBoxController.exec(tr("FTP"), tr("Unable to connect to the FTP server "
                    "at %1. Please check that the host "
                    "name is correct.").arg(m_url),
                    tr("Ok"), QString());
            connectOrDisconnect();
            return;
        }

        // ... otherwise update status properties to reflect successful login.
        m_statusText = tr("Logged onto %1.").arg(m_url);
        emit statusTextChanged();

        m_connectPossible = true;
        emit connectPossibleChanged();

        return;
    }
//![6]

//![7]
    // After a successful login we immediately start with the directory listing
    if (m_ftp->currentCommand() == QFtp::Login)
        m_ftp->list();
//![7]

//![8]
    if (m_ftp->currentCommand() == QFtp::Get) {
        if (error) {
            // If the download failed, we update the status property ...
            m_statusText = tr("Canceled download of %1.").arg(m_downloadFileName);
            emit statusTextChanged();

            // ... and clean up the local target file
            if (m_file) {
                m_file->close();
                m_file->remove();
            }
        } else {
            // If the download succeeded or was explicitly canceled, we just update the status property
            if (m_file) {
                const QFileInfo actualDir(*m_file);
                m_statusText = tr("Downloaded %1 to %2.").arg(m_downloadFileName).arg(actualDir.absolutePath());
                emit statusTextChanged();

                m_file->close();
            } else {
                m_statusText = tr("Canceled download of %1.").arg(m_downloadFileName);
                emit statusTextChanged();
            }
        }

        // Delete the target file object (Note: that does not delete the local file on the file system)
        delete m_file;
        m_file = 0;

        // Update the status properties
        enableDownloadButton();

        // Hide the progress dialog
        m_progressDialogController.hide();
//![8]
//![9]
    } else if (m_ftp->currentCommand() == QFtp::List) {
        if (m_model.isEmpty()) {
            /**
             * If the model is empty after a directory listing, we add a not selectable dummy entry, so
             * that the user has some visual feedback for an empty directory.
             */
            FtpItem item;
            item.fileName = tr("<empty>");
            m_model.append(item);
            m_selectionPossible = false;
            emit selectionPossibleChanged();
        }

        if (!m_currentPath.isEmpty()) {
            // Update the status property
            m_parentDirectoryAvailable = true;
            emit parentDirectoryAvailableChanged();
        }
    }
//![9]
}

//![10]
void FtpDownloader::addToList(const QUrlInfo &urlInfo)
{
    // Fill an FtpItem with the data from FTP directory listing ...
    FtpItem item;
    item.fileName = urlInfo.name();
    item.fileSize = urlInfo.size();
    item.owner = urlInfo.owner();
    item.group = urlInfo.group();
    item.time = urlInfo.lastModified();
    item.isDirectory = urlInfo.isDir();

    const bool wasEmpty = m_model.isEmpty();

    // ... and append it to the model
    m_model.append(item);

    // If this is the first entry in the model, also update the status property
    if (wasEmpty) {
        m_selectionPossible = true;
        emit selectionPossibleChanged();
    }
}
//![10]

//![11]
void FtpDownloader::processItem(const QVariantList &indexPath)
{
    if (!m_selectionPossible)
        return;

    m_currentIndexPath = indexPath;
    enableDownloadButton();

    if (!indexPath.isEmpty()) {
        // If the user has selected an valid item in the directory listing ListView, check whether it's a directory
        const FtpItem item = m_model.data(indexPath).value<FtpItem>();
        if (item.isDirectory) {
            // In this case clear the content of the model ...
            m_model.clear();

            // ... assemble the path for the selected subdirectory ...
            m_currentPath += '/';
            m_currentPath += item.fileName;

            // ... and trigger a listing for this subdirectory
            m_ftp->cd(item.fileName);
            m_ftp->list();

            // Update the status property
            m_parentDirectoryAvailable = true;
            emit parentDirectoryAvailableChanged();
            return;
        }
    }
}
//![11]

//![12]
void FtpDownloader::cdToParent()
{
    // If the user requests to go to the parent directory clear the model ...
    m_model.clear();

    m_currentIndexPath.clear();
    enableDownloadButton();

    // ... extract the path to the parent directory ...
    m_currentPath = m_currentPath.left(m_currentPath.lastIndexOf('/'));

    // ... change to the parent directory ...
    if (m_currentPath.isEmpty()) {
        m_parentDirectoryAvailable = false;
        emit parentDirectoryAvailableChanged();
        m_ftp->cd("/");
    } else {
        m_ftp->cd(m_currentPath);
    }

    // ... and trigger a listing for the parent directory
    m_ftp->list();
}
//![12]

//![13]
void FtpDownloader::updateDataTransferProgress(qint64 readBytes, qint64 totalBytes)
{
    // Calculate the progress of the download in percent
    const float progress = readBytes / (float) totalBytes * 100.0f;

    // Update the progress information in the progress dialog
    m_progressDialogController.setProgress(progress);
}
//![13]

//![14]
void FtpDownloader::enableDownloadButton()
{
    // Check whether the currently selected directory listing entry can be downloaded
    const QVariant itemData = m_model.data(m_currentIndexPath);
    if (itemData.isValid()) {
        const FtpItem item = itemData.value<FtpItem>();
        m_downloadPossible = !item.isDirectory;
    } else {
        m_downloadPossible = false;
    }
    emit downloadPossibleChanged();
}
//![14]

void FtpDownloader::enableConnectButton()
{
    // Save the used configuration
    QNetworkConfiguration config = m_networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = m_networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();

    // Update the status properties
    m_connectPossible = true;
    emit connectPossibleChanged();

    m_statusText = tr("Please enter the name of an FTP server.");
    emit statusTextChanged();
}

bb::cascades::QListDataModel<FtpItem> *FtpDownloader::model()
{
    return &m_model;
}

MessageBoxController *FtpDownloader::messageBoxController()
{
    return &m_messageBoxController;
}

ProgressDialogController *FtpDownloader::progressDialogController()
{
    return &m_progressDialogController;
}
