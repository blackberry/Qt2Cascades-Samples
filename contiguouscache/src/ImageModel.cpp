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

#include "ImageModel.hpp"

using namespace bb::cascades;

// The number of Images the cache will keep in memory at a time
static const int bufferSize(50);

// The number of Images it will pre-load even if the view has not requested them yet
static const int lookAhead(10);

// The number of Images it will pre-load before and after the current element
static const int halfLookAhead(lookAhead/2);

ImageModel::ImageModel(QObject *parent)
    : DataModel(parent)
    , m_rows(bufferSize)
    , m_count(10000)
{
}

int ImageModel::childCount(const QVariantList &indexPath)
{
    if ( indexPath.isEmpty() )
        return m_count;
    else {
        return 0;
    }
}

bool ImageModel::hasChildren(const QVariantList &indexPath)
{
    // We have only one level of items (plain list)
    return indexPath.isEmpty();
}

//![0]
QVariant ImageModel::data(const QVariantList &indexPath)
{
    if (indexPath.isEmpty())
        return QVariant();

    // Extract the requested row from the index path
    const int row = indexPath.first().toInt();

    // Store the current number of cached items
    const int oldCachedItemCount = m_rows.count();

    if (row > m_rows.lastIndex()) { // If a row after the last valid index in cache is requested ...
        if (row - m_rows.lastIndex() > lookAhead) { // ... and the distance is larger than the look ahead ...
            // ... fill the cache with values around the requested row
            cacheRows(row-halfLookAhead, qMin(m_count, row+halfLookAhead));
        } else {
            // ... otherwise append new values to the cache until the requested row is reached
            while (row > m_rows.lastIndex()) {
                m_rows.append(fetchRow(m_rows.lastIndex()+1));
            }
        }
    } else if (row < m_rows.firstIndex()) { // If a row before the first valid index in cache is requested ...
        if (m_rows.firstIndex() - row > lookAhead) { // ... and the distance is larger than the look ahead ...
            // ... fill the cache with values around the requested row
            cacheRows(qMax(0, row-halfLookAhead), row+halfLookAhead);
        } else {
            // ... otherwise prepend new values to the cache until the requested row is reached
            while (row < m_rows.firstIndex()) {
                m_rows.prepend(fetchRow(m_rows.firstIndex()-1));
            }
        }
    }

    // If the number of cached item has changed, emit the signal
    if (oldCachedItemCount != m_rows.count())
        emit cachedItemCountChanged();

    // Return the Image object for the requested row
    return QVariant::fromValue(m_rows.at(row));
}
//![0]

QString ImageModel::itemType(const QVariantList &indexPath)
{
    Q_UNUSED(indexPath);

    // This model provides only items, no headers
    return QString();
}

int ImageModel::cachedItemCount() const
{
    return m_rows.count();
}

//![1]
void ImageModel::cacheRows(int from, int to) const
{
    /**
     * We simply fill the cache with new values for the given range, where the
     * construction of the new values is encapsulated in the fetchRow() method.
     */
    for (int i = from; i <= to; ++i)
        m_rows.insert(i, fetchRow(i));
}
//![1]

//![2]
Image ImageModel::fetchRow(int position) const
{
    Q_UNUSED(position);

    return Image(QUrl(QString::fromLatin1("asset:///images/text%1.png").arg(position%10)));
}
//![2]
