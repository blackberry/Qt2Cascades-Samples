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

#include "CertificateInfoControl.hpp"

CertificateInfoControl::CertificateInfoControl(QObject *parent)
    : QObject(parent), m_visible(false)
{
}

void CertificateInfoControl::setCertificateChain(const QList<QSslCertificate> &chain)
{
    m_chain = chain;

    // Clear the model ...
    m_model.clear();

    // ... and add a new entry for each certificate from the chain to the model
    for (int i = 0; i < m_chain.size(); ++i) {
        const QSslCertificate &cert = m_chain.at(i);
        const QString chainInformation = tr("%1%2 (%3)").arg(!i ? QString() : tr("Issued by: "))
                .arg(cert.subjectInfo(QSslCertificate::Organization))
                .arg(cert.subjectInfo(QSslCertificate::CommonName));

        m_model.append(chainInformation);
    }

    // Notify the UI that the model has changed
    emit modelChanged();
    emit listItemManagerChanged();

    // Pre-select the first certificate in the list
    setCurrentCertificate(QVariantList() << QVariant(0));
}

bb::cascades::DataModel* CertificateInfoControl::model() const
{
    return const_cast<bb::cascades::QListDataModel<QString>*>(&m_model);
}

bb::cascades::ListItemManager* CertificateInfoControl::listItemManager() const
{
    return const_cast<CertificateItemManager*>(&m_listItemManager);
}

QString CertificateInfoControl::certificateInfo() const
{
    return m_certificateInfo;
}

void CertificateInfoControl::setVisible(bool visible)
{
    if (m_visible == visible)
        return;

    m_visible = visible;
    emit visibleChanged();
}

bool CertificateInfoControl::visible() const
{
    return m_visible;
}

void CertificateInfoControl::show()
{
    setVisible(true);
}

void CertificateInfoControl::setCurrentCertificate(const QVariantList &indexPath)
{
    // Get the index of the certificate that the user has selected
    const int index = indexPath.at(0).toInt();

    if (index >= 0 && index < m_chain.size()) {
        const QSslCertificate &cert = m_chain.at(index);

        // Assemble the textual representation of the selected certificate and update the property
        QStringList lines;
        lines << tr("Organization: %1").arg(cert.subjectInfo(QSslCertificate::Organization))
                << tr("Subunit: %1").arg(cert.subjectInfo(QSslCertificate::OrganizationalUnitName))
                << tr("Country: %1").arg(cert.subjectInfo(QSslCertificate::CountryName))
                << tr("Locality: %1").arg(cert.subjectInfo(QSslCertificate::LocalityName))
                << tr("State/Province: %1").arg(cert.subjectInfo(QSslCertificate::StateOrProvinceName))
                << tr("Common Name: %1").arg(cert.subjectInfo(QSslCertificate::CommonName))
                << QString()
                << tr("Issuer Organization: %1").arg(cert.issuerInfo(QSslCertificate::Organization))
                << tr("Issuer Unit Name: %1").arg(cert.issuerInfo(QSslCertificate::OrganizationalUnitName))
                << tr("Issuer Country: %1").arg(cert.issuerInfo(QSslCertificate::CountryName))
                << tr("Issuer Locality: %1").arg(cert.issuerInfo(QSslCertificate::LocalityName))
                << tr("Issuer State/Province: %1").arg(cert.issuerInfo(QSslCertificate::StateOrProvinceName))
                << tr("Issuer Common Name: %1").arg(cert.issuerInfo(QSslCertificate::CommonName));

        m_certificateInfo = lines.join("\n");
    } else {
        m_certificateInfo.clear();
    }

    emit certificateInfoChanged();
}
