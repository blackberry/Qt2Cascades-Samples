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

#ifndef AUTHENTICATIONDIALOGCONTROLLER_HPP
#define AUTHENTICATIONDIALOGCONTROLLER_HPP

#include "DialogController.hpp"
//! [0]
/**
 * The AuthenticationDialogController encapsulates the logic of
 * a password dialog. When executed it will block the execution flow
 * until the user has entered a user name and password.
 */
class AuthenticationDialogController : public DialogController
{
    Q_OBJECT

    // Makes the site, the credentials are requested for, available to the UI
    Q_PROPERTY(QString site READ site NOTIFY siteChanged)

    // Makes the user name of the credentials available to the UI
    Q_PROPERTY(QString user READ user NOTIFY userChanged)

    // Makes the password of the credentials available to the UI
    Q_PROPERTY(QString password READ password NOTIFY passwordChanged)

public:
    // The accessor methods for the properties
    QString site() const;
    QString user() const;
    QString password() const;

    // This method is executed to show the dialog
    bool exec(const QString &site, const QString &user, const QString &password);

    // This method is called from the UI when the user has entered a user name and password
    Q_INVOKABLE void continueAuthentication(const QString &user, const QString &password);

    // This method is called from the UI when the user wants to cancel authentication
    Q_INVOKABLE void cancel();

Q_SIGNALS:
    // The change notification signals of the properties
    void siteChanged();
    void userChanged();
    void passwordChanged();

private:
    // The property values
    QString m_site;
    QString m_user;
    QString m_password;

    // A flag that stores whether dialog has been canceled
    bool m_result;
};
//! [0]
#endif
