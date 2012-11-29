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

#ifndef SSLCLIENT_HPP
#define SSLCLIENT_HPP

#include "SslErrorControl.hpp"

#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QSslSocket>

class QSslSocket;

/**
 * The SslClient encapsulates the business logic of this application.
 * It handles the low-level communication with the server via QSslSocket and
 * mediates the input and output with the UI.
 */
class SslClient : public QObject
{
    Q_OBJECT

    // The host name of the server to connect to
    Q_PROPERTY(QString hostName READ hostName WRITE setHostName NOTIFY hostNameChanged)

    // The port number of the server to connect to
    Q_PROPERTY(QString port READ port WRITE setPort NOTIFY portChanged)

    // Reports whether a SSL session has been established
    Q_PROPERTY(bool sessionActive READ sessionActive NOTIFY sessionActiveChanged)

    // The name of the cipher that is used by the SSL connection
    Q_PROPERTY(QString cipher READ cipher NOTIFY cipherChanged)

    // The response data that are send by the server
    Q_PROPERTY(QString response READ response NOTIFY responseChanged)

public:
    SslClient(QObject *parent = 0);

    // The accessor methods of the properties
    void setHostName(const QString &hostName);
    QString hostName() const;

    void setPort(const QString &port);
    QString port() const;

    bool sessionActive() const;

    QString cipher() const;

    QString response() const;

    // Returns the controller for the SSL error dialog
    SslErrorControl* sslErrorControl() const;

public Q_SLOTS:
    // This method is called from the UI to create a secure connection
    void secureConnect();

    // This method is called from the UI to send data over the secure connection to the server
    void sendData(const QString &data);

Q_SIGNALS:
    // The change notification signals of the properties
    void hostNameChanged();
    void portChanged();
    void sessionActiveChanged();
    void cipherChanged();
    void responseChanged();

    // This signal is emitted whenever the server reports the certificate chain that should be used
    void certificateChainChanged(const QList<QSslCertificate> &chain);

    // This signal is emitted if the user requested to view the certificate chain
    void viewCertificateChainRequested();

private Q_SLOTS:
    void updateEnabledState();

    // This method is called whenever the state of the SSL socket has changed
    void socketStateChanged(QAbstractSocket::SocketState state);

    // This method is called after an successful SSL-handshake
    void socketEncrypted();

    // This method is called when data arrived from the server via the secured connection
    void socketReadyRead();

    // This method is called when the SSL socket reports SSL-related errors
    void sslErrors(const QList<QSslError> &errors);

    // This method is called if the user wants to ignore SSL-related errors
    void ignoreSslErrors();

private:
    // This methods adds a response from the server to the response property
    void appendString(const QString &line);

    // The SSL socket that does the low-level communication
    QSslSocket *m_socket;

    // The controller for the SSL error dialog
    SslErrorControl *m_sslErrorControl;

    // The property values
    QString m_hostName;
    int m_port;
    bool m_sessionActive;
    QString m_cipher;
    QString m_response;
};

#endif
