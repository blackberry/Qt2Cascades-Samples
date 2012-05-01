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

#ifndef SENDER_HPP
#define SENDER_HPP

#include <QtCore/QObject>
#include <QtNetwork/QHostAddress>

class QTimer;
class QUdpSocket;

/**
 * The Sender class sends UDP multicast datagrams every second and makes a status
 * message available to the UI via the 'status' property.
 * The TTL of the datagrams can be changed by the user through the 'ttl' property.
 */
class Sender : public QObject
{
    Q_OBJECT

    // The property that makes the status message available to the UI
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)

    // The property that allows the user to change the TTL of the datagrams
    Q_PROPERTY(QString ttl READ ttl WRITE setTtl NOTIFY ttlChanged)

public:
    Sender(QObject *parent = 0);

public Q_SLOTS:
    // This method is called when the user clicks the 'Start' button in the UI
    void startSending();

Q_SIGNALS:
    // The change notification signal of the properties
    void statusChanged();
    void ttlChanged();

private Q_SLOTS:
    // This method is called every second to send the next multicast datagram
    void sendDatagram();

private:
    // The accessor methods of the properties
    QString status() const;
    QString ttl() const;
    void setTtl(const QString &ttl);

    // The UDP socket object
    QUdpSocket *m_udpSocket;

    // The timer that triggers the sending of the multicast datagrams
    QTimer *m_timer;

    // The multicast address
    QHostAddress m_groupAddress;

    // A counter for the number of sent datagrams
    int m_messageNo;

    // The status message
    QString m_status;

    // The time-to-live that is used for the datagrams
    int m_ttl;
};

#endif
