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

#include "Receiver.hpp"

#include <QtNetwork/QUdpSocket>

Receiver::Receiver(QObject *parent)
    : QObject(parent)
{
    // Initialize the multicast address
    m_groupAddress = QHostAddress("239.255.43.21");

    /**
     * Initialize the 'status' property with some data, it will
     * be shown in the UI until the first datagram arrives.
     */
    m_status = tr("Listening for multicasted messages");

    // Create a new UDP socket and bind it against port 45454
    m_udpSocket = new QUdpSocket(this);
    m_udpSocket->bind(45454, QUdpSocket::ShareAddress);

    // Tell the UDP socket which multicast group it should join
    m_udpSocket->joinMulticastGroup(m_groupAddress);

    /**
     * Create signal/slot connection to invoke processPendingDatagrams() whenever
     * a new multicast datagram is received by the socket.
     */
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}

void Receiver::processPendingDatagrams()
{
    // Now read all available datagrams from the socket
    while (m_udpSocket->hasPendingDatagrams()) {
        // Create a temporary buffer ...
        QByteArray datagram;

        // ... with the size of the received multicast datagram ...
        datagram.resize(m_udpSocket->pendingDatagramSize());

        // ... and copy over the received multicast datagram into that buffer.
        m_udpSocket->readDatagram(datagram.data(), datagram.size());

        // Update the 'status' property with the content of the received multicast datagram
        m_status = tr("Received datagram: \"%1\"").arg(datagram.data());
        emit statusChanged();
    }
}

QString Receiver::status() const
{
    return m_status;
}
