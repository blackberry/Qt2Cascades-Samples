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

//! [0]
Sender::Sender(QObject *parent)
    : QObject(parent)
{
    /**
     * Initialize the 'status' property with no data, it won't
     * be shown in the UI until the first datagram is sent.
     */
    m_status = "";

    // Create the timer that triggers the broadcast of the datagrams
    m_timer = new QTimer(this);

    /**
     * Create the UDP socket object.
     * Note: In opposite to a TCP socket we don't have to establish a network connection.
     */
    m_udpSocket = new QUdpSocket(this);

    // Initialize the datagram counter
    m_messageNo = 1;

    /**
     * Create a signal/slot connection so that whenever the timer times out, the broadcastDatagram()
     * method is invoked.
     */
    connect(m_timer, SIGNAL(timeout()), this, SLOT(broadcastDatagram()));
}
//! [0]

//! [1]
void Sender::startBroadcasting()
{
    // Start the timer with an interval of 1 second
    m_timer->start(1000);
}
//! [1]

//! [2]
void Sender::broadcastDatagram()
{
    // Update the status message and signal that it has changed
    m_status = tr("%1").arg(m_messageNo);
    emit statusChanged();

    // Assemble the content of the datagram
    const QByteArray datagram = "Broadcast message "
            + QByteArray::number(m_messageNo);

    // Broadcast a datagram with the given content to port 45454
    m_udpSocket->writeDatagram(datagram.data(), datagram.size(),
            QHostAddress::Broadcast, 45454);

    // Increase the datagram counter, so that the next sent datagram will have a different content
    ++m_messageNo;
}
//! [2]

QString Sender::status() const
{
    return m_status;
}
