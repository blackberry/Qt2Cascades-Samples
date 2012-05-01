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

#include "FtpItemManager.hpp"
#include "FtpItem.hpp"
#include "FtpListItem.hpp"

#include <bb/cascades/Image>
#include <bb/cascades/ListView>
#include <bb/cascades/StandardListItem>

#include <QtCore/QUrl>

using namespace bb::cascades;

FtpItemManager::FtpItemManager(QObject *parent)
    : ListItemManager(parent), m_fileImage(QUrl("asset:///file.png")), m_folderImage(QUrl("asset:///folder.png"))
{
}

bb::cascades::VisualNode* FtpItemManager::createItem(bb::cascades::ListView*, const QString&)
{
    // Return an instance of our custom FtpListItem class
    return new FtpListItem();
}

/**
 * This methods converts a numeric file/directory size into a human readable string
 * that includes a unit.
 */
static QString humanReadableFileSize(qint64 size)
{
    const QStringList units = QStringList() << QObject::tr("KB")
            << QObject::tr("MB")
            << QObject::tr("GB")
            << QObject::tr("TB");
    QString unit(QObject::tr("bytes"));
    QStringListIterator unitIterator(units);

    float num = size;
    while (num >= 1024 && unitIterator.hasNext()) {
        unit = unitIterator.next();
        num /= 1024.0f;
    }
    if (num == size) {
        return QObject::tr("%1 %2", "<size> <unit>").arg(size).arg(unit);
    } else {
        return QObject::tr("%1 %2", "<size> <unit>").arg(QString::number(num, 'f', 2)).arg(unit);
    }
}

void FtpItemManager::updateItem(bb::cascades::ListView*, bb::cascades::VisualNode *control, const QString&,
                                const QVariantList&, const QVariant &data)
{
    // Extract the FtpItem object from the model data
    const FtpItem item = data.value<FtpItem>();

    // Cast the generic control to the specific subclass (the one we returned in createItem() above)
    FtpListItem * guiItem = qobject_cast<FtpListItem*>(control);

    // Fill the ListView item with the data from the data model
    guiItem->setTitleText(item.fileName);

    const QString commonAttributes = tr("Owner: %1, Group: %2, Last Modified: %3").arg(item.owner).arg(item.group).arg(item.time.toString());
    if (item.isDirectory) {
        guiItem->setImage(m_folderImage);
        guiItem->setStatusText(commonAttributes);
    } else {
        guiItem->setImage(m_fileImage);
        guiItem->setStatusText(tr("Size: %1, %2)").arg(humanReadableFileSize(item.fileSize)).arg(commonAttributes));
    }
}
