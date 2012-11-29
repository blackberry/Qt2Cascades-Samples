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

#ifndef SSLERRORCONTROL_HPP
#define SSLERRORCONTROL_HPP

#include <QtCore/QObject>
#include <QtCore/QEventLoop>

/**
 * The SslErrorControl represents an dialog for SSL-related error messages.
 * It is normally shown if the server provides a SSL certificate that can not
 * be verified by the client. In this case the user is asked for accept the
 * certificate or abort the connection.
 */
class SslErrorControl : public QObject
{
    Q_OBJECT

    // Reports whether this dialog is visible or not
    Q_PROPERTY(bool visible READ visible NOTIFY visibleChanged)

    // Makes the message of the dialog available to the UI
    Q_PROPERTY(QString message READ message NOTIFY messageChanged)

public:
    SslErrorControl(QObject *parent = 0);

    // Executes the dialog and shows the given error message
    void exec(const QString &message);

    // The accessor methods of the properties
    bool visible() const;
    QString message() const;

public Q_SLOTS:
    // This method is invoked from the UI if the user wants to ignore the SSL errors
    void ignore();

    // This method is invoked from the UI if the user wants to cancel the connection
    void cancel();

    // This method is invoked from the UI if the user wants to view the certificate chain
    void viewCertificateChain();

Q_SIGNALS:
    // The change notification signals of the properties
    void visibleChanged();
    void messageChanged();

    // This signal is emitted when the user wants to ignore the SSL errors
    void ignoreSslErrors();

    // This signal is emitted when the user wants to view the certificate chain
    void viewCertificateChainRequested();

private:
    // The nested event loop that is used to block execution flow while the dialog is shown
    QEventLoop m_eventLoop;

    // The property values
    bool m_ignoreSslErrors;
    bool m_visible;
    QString m_message;
};

#endif
