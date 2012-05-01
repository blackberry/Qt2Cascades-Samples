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

#include "Sender.hpp"

#include <QtCore/QTimer>
#include <QtNetwork/QUdpSocket>

Sender::Sender(QObject *parent)
    : QObject(parent), m_ttl(0)
{
    // Initialize the multicast address
    m_groupAddress = QHostAddress("239.255.43.21");

    /**
     * Initialize the 'status' property with some data, it will
     * be shown in the UI until the first datagram is sent.
     */
    m_status = tr("Ready to multicast datagrams to group %1 on port 45454").arg(m_groupAddress.toString());

    // Create the timer that triggers the sending of the multicast datagrams
    m_timer = new QTimer(this);

    // Create the UDP socket object.
    m_udpSocket = new QUdpSocket(this);

    // Initialize the datagram counter
    m_messageNo = 1;

    // Initialize the TTL
    setTtl("1");

    /**
     * Create a signal/slot connection so that whenever the timer times out, the sendDatagram()
     * method is invoked.
     */
    connect(m_timer, SIGNAL(timeout()), this, SLOT(sendDatagram()));
}

void Sender::startSending()
{
    // Start the timer with an interval of 1 second
    m_timer->start(1000);
}

void Sender::sendDatagram()
{
    // Update the status message and signal that it has changed
    m_status = tr("Now sending datagram %1").arg(m_messageNo);
    emit statusChanged();

    // Assemble the content of the datagram
    const QByteArray datagram = "Multicast message " + QByteArray::number(m_messageNo);

    // Send the multicast datagram with the given content to port 45454
    m_udpSocket->writeDatagram(datagram.data(), datagram.size(), m_groupAddress, 45454);

    // Increase the datagram counter, so that the next sent datagram will have a different content
    ++m_messageNo;
}

QString Sender::status() const
{
    return m_status;
}

QString Sender::ttl() const
{
    return QString::number(m_ttl);
}

void Sender::setTtl(const QString &ttlString)
{
    bool ok = false;

    // Convert the input string to an integer value
    const int ttl = ttlString.toInt(&ok);

    // Ignore invalid values or if the new TTL is the same as the current one
    if (!ok || (m_ttl == ttl))
        return;

    // Update the internal TTL value ...
    m_ttl = ttl;

    // ... and set it on the multicast UDP socket.
    m_udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption, m_ttl);

    emit ttlChanged();
}
