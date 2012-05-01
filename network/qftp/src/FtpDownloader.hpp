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

#ifndef FTPDOWNLOADER_H_
#define FTPDOWNLOADER_H_

#include "FtpItem.hpp"
#include "MessageBoxController.hpp"
#include "ProgressDialogController.hpp"

#include <bb/cascades/QListDataModel>

#include <QtCore/QObject>

namespace bb
{
namespace cascades
{
class Control;
class QmlDocument;
}
}

/**
 * The FtpDownloader contains all the business logic of this application.
 * It is responsible for connecting to a FTP server, list the content of a directory
 * on the server, navigate through the file system on the FTP server and download
 * a file that has been selected by the user.
 *
 * The low-level FTP communication is done by QFtp and network session management by
 * QNetworkConfigurationManager.
 */
class FtpDownloader : public QObject
{
    Q_OBJECT

    // Makes the URL of the FTP server available to the UI
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)

    // Makes the status message available to the UI
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)

    // Makes the connect message available to the UI
    Q_PROPERTY(QString connectLabel READ connectLabel NOTIFY connectLabelChanged)

    /**
     * The following properties tell the UI about the current state of the FtpDownloader.
     * These information are used to enabled/disable certain controls in the UI.
     */
    Q_PROPERTY(bool parentDirectoryAvailable READ parentDirectoryAvailable NOTIFY parentDirectoryAvailableChanged)
    Q_PROPERTY(bool downloadPossible READ downloadPossible NOTIFY downloadPossibleChanged)
    Q_PROPERTY(bool connectPossible READ connectPossible NOTIFY connectPossibleChanged)
    Q_PROPERTY(bool selectionPossible READ selectionPossible NOTIFY selectionPossibleChanged)

public:
    explicit FtpDownloader(QObject *parent = 0);
    ~FtpDownloader();

    // Returns the model that contains the entries of a directory listing
    bb::cascades::QListDataModel<FtpItem> *model();

    // The accessor methods for the dialog controllers
    MessageBoxController *messageBoxController();
    ProgressDialogController *progressDialogController();

public Q_SLOTS:
    // This method is called from the UI to change to the parent directory when navigating on the FTP server
    void cdToParent();

    // This method is called from the UI to start the download of a file from the FTP server
    void downloadFile();

    // This method is called from the UI to establish or terminate a connection to an FTP server
    void connectOrDisconnect();

    // This method is called whenever the user has selected an item in the directory listing ListView
    void processItem(const QVariantList &indexPath, bool active);

Q_SIGNALS:
    // The change notification signals of the properties
    void urlChanged();
    void statusTextChanged();
    void parentDirectoryAvailableChanged();
    void downloadPossibleChanged();
    void connectPossibleChanged();
    void connectLabelChanged();
    void selectionPossibleChanged();

private Q_SLOTS:
    // This method is called when the user canceled the download in the progress dialog
    void cancelDownload();

    // This method is called whenever a FTP command has been processed
    void ftpCommandFinished(int commandId, bool error);

    // This method is called whenever a FTP directory listing returns new items
    void addToList(const QUrlInfo &urlInfo);

    // This method is called whenever the progress of an FTP download has changed
    void updateDataTransferProgress(qint64 readBytes, qint64 totalBytes);

    // This method is called to signal that a download is possible
    void enableDownloadButton();

    // This method is called to signal that connecting to a FTP server is possible
    void enableConnectButton();

private:
    // The accessor methods of the properties
    void setUrl(const QString &url);
    QString url() const;
    QString statusText() const;
    bool parentDirectoryAvailable() const;
    bool downloadPossible() const;
    bool connectPossible() const;
    QString connectLabel() const;
    bool selectionPossible() const;

    // Establishe the connection to the FTP server
    void connectToFtp();

    // The data model that stores the items of a FTP directory listing
    bb::cascades::QListDataModel<FtpItem> m_model;

    // The URL of the FTP server
    QString m_url;

    // The current path we are navigating to on the FTP server
    QString m_currentPath;

    // The FTP client object
    QFtp *m_ftp;

    // The file to download the data to
    QFile *m_file;

    // The name of the file to download data to
    QString m_downloadFileName;

    // A textual description of the status
    QString m_statusText;

    // A flag for whether a parent directory is available on the FTP server
    bool m_parentDirectoryAvailable;

    // A flag for whether we can currently start a new download
    bool m_downloadPossible;

    // A flag for whether we can connect to an FTP server
    bool m_connectPossible;

    // A textual description of the connection status
    QString m_connectLabel;

    // A flag for whether we can select a file in the directory listing ListView
    bool m_selectionPossible;

    // The index of the currently selected item in the directory listing ListView
    QVariantList m_currentIndexPath;

    // The helper objects to handle network connectivity status
    QNetworkSession *m_networkSession;
    QNetworkConfigurationManager m_manager;

    // The dialog controllers
    MessageBoxController m_messageBoxController;
    ProgressDialogController m_progressDialogController;
};

#endif
