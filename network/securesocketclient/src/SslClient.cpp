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

#include "SslClient.hpp"

#include <QtCore/QStringList>
#include <QtNetwork/QSslCipher>

SslClient::SslClient(QObject *parent)
    : QObject(parent)
    , m_socket(0)
    , m_sslErrorControl(new SslErrorControl(this))
    , m_port(443)
    , m_sessionActive(false)
    , m_cipher("<none>")
{
    // User input to the SSL error dialog is forwarded to us via signals
    connect(m_sslErrorControl, SIGNAL(ignoreSslErrors()),
            this, SLOT(ignoreSslErrors()));

    connect(m_sslErrorControl, SIGNAL(viewCertificateChainRequested()),
            this, SIGNAL(viewCertificateChainRequested()));
}

void SslClient::setHostName(const QString &hostName)
{
    if (m_hostName == hostName)
        return;

    m_hostName = hostName;
    emit hostNameChanged();

    updateEnabledState();
}

QString SslClient::hostName() const
{
    return m_hostName;
}

void SslClient::setPort(const QString &portString)
{
    bool ok = false;
    const int port = portString.toInt(&ok);

    if (!ok || (m_port == port))
        return;

    m_port = port;
    emit portChanged();
}

QString SslClient::port() const
{
    return QString::number(m_port);
}

bool SslClient::sessionActive() const
{
    return m_sessionActive;
}

QString SslClient::cipher() const
{
    return m_cipher;
}

QString SslClient::response() const
{
    return m_response;
}

SslErrorControl* SslClient::sslErrorControl() const
{
    return m_sslErrorControl;
}

void SslClient::updateEnabledState()
{
    // Update the sessionActive property depending on the current SSL socket state

    const bool sessionActive = (m_socket && (m_socket->state() == QAbstractSocket::ConnectedState));
    if (m_sessionActive == sessionActive)
        return;

    m_sessionActive = sessionActive;
    emit sessionActiveChanged();
}

void SslClient::secureConnect()
{
    if (!m_socket) {
        // Create a new SSL socket and connect against its signals to receive notifications about state changes
        m_socket = new QSslSocket(this);
        connect(m_socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                this, SLOT(socketStateChanged(QAbstractSocket::SocketState)));
        connect(m_socket, SIGNAL(encrypted()),
                this, SLOT(socketEncrypted()));
        connect(m_socket, SIGNAL(sslErrors(QList<QSslError>)),
                this, SLOT(sslErrors(QList<QSslError>)));
        connect(m_socket, SIGNAL(readyRead()),
                this, SLOT(socketReadyRead()));
    }

    // Trigger the SSL-handshake
    m_socket->connectToHostEncrypted(m_hostName, m_port);

    updateEnabledState();
}

void SslClient::socketStateChanged(QAbstractSocket::SocketState state)
{
    if (m_sslErrorControl->visible())
        return; // We won't react to state changes while the SSL error dialog is visible

    updateEnabledState();

    if (state == QAbstractSocket::UnconnectedState) {
        // If the SSL socket has been disconnected, we delete the socket
        m_cipher = "<none>";
        emit cipherChanged();

        m_socket->deleteLater();
        m_socket = 0;
    }
}

void SslClient::socketEncrypted()
{
    if (!m_socket)
        return; // Might have disconnected already

    // We started a new connection, so clear the response from previous connections
    m_response.clear();
    emit responseChanged();

    // Retrieve the information about the used cipher and update the property
    const QSslCipher cipher = m_socket->sessionCipher();
    m_cipher = QString("%1, %2 (%3/%4)").arg(cipher.authenticationMethod())
            .arg(cipher.name())
            .arg(cipher.usedBits())
            .arg(cipher.supportedBits());

    emit cipherChanged();

    // Tell the CertificateInfoControl about the certificate chain of this connection
    emit certificateChainChanged(m_socket->peerCertificateChain());
}

void SslClient::socketReadyRead()
{
    // Read the response from the server and append it to the 'response' property
    appendString(QString::fromUtf8(m_socket->readAll()));
}

void SslClient::sendData(const QString &input)
{
    if (input.isEmpty())
        return;

    // Add an additional line break, some protocols need that
    appendString(input + '\n');

    // Send the data to the server
    m_socket->write(input.toUtf8() + "\r\n");
}

void SslClient::sslErrors(const QList<QSslError> &errors)
{
    // Assemble the error message, ...
    QStringList messages;
    foreach (const QSslError &error, errors)
        messages << error.errorString();

    // ... make sure the CertificateInfoControl knows about the certificate chain, ...
    emit certificateChainChanged(m_socket->peerCertificateChain());

    // ... and show the SSL error dialog.
    m_sslErrorControl->exec(messages.join("\n"));

    // If the socket has been disconnected (while we have shown the SSL error dialog) we have to update the state
    if (m_socket && (m_socket->state() != QAbstractSocket::ConnectedState))
        socketStateChanged(m_socket->state());
}

void SslClient::ignoreSslErrors()
{
    if (m_socket)
        m_socket->ignoreSslErrors();
}

void SslClient::appendString(const QString &line)
{
    // Update the 'response' property
    m_response += line;
    emit responseChanged();
}
