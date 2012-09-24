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

#ifndef CERTIFICATEINFOCONTROL_HPP
#define CERTIFICATEINFOCONTROL_HPP

#include <bb/cascades/QListDataModel>

#include <QtCore/QObject>
#include <QtNetwork/QSslCertificate>

/**
 * The CertificateInfoControl represents a dialog that shows information
 * about the certificate provided by the server and the certificate chain.
 */
class CertificateInfoControl: public QObject
{
    Q_OBJECT

    // Makes the list of certificates (certificate chain) available as DataModel to the UI
    Q_PROPERTY(bb::cascades::DataModel* model READ model CONSTANT)

    // Makes a textual representation of the selected certificate available to the UI
    Q_PROPERTY(QString certificateInfo READ certificateInfo NOTIFY certificateInfoChanged)

    // Reports whether this dialog is visible or not
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)

public:
    CertificateInfoControl(QObject *parent = 0);

    // The accessor methods of the properties
    bb::cascades::DataModel* model() const;

    QString certificateInfo() const;

    void setVisible(bool visible);
    bool visible() const;

public Q_SLOTS:
    // This method is called to show the dialog
    void show();

    // This method is called to set certificate chain that should be shown in the dialog
    void setCertificateChain(const QList<QSslCertificate> &chain);

    // This method is called when the user selects a certificate in the ListView
    void setCurrentCertificate(const QVariantList &indexPath);

Q_SIGNALS:
    // The change notification signals of the properties
    void certificateInfoChanged();
    void visibleChanged();

private:
    // The certificates of the certificate chain
    QList<QSslCertificate> m_chain;

    // The DataModel that provides the certificate chain to the ListView
    bb::cascades::QListDataModel<QString> m_model;

    // The textual representation of the currently selected certificate
    QString m_certificateInfo;

    // A flag for whether the dialog is currently visible
    bool m_visible;
};

#endif
