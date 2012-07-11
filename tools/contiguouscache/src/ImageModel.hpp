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

#ifndef IMAGEMODEL_HPP
#define IMAGEMODEL_HPP

#include <bb/cascades/DataModel>
#include <bb/cascades/Image>

#include <QContiguousCache>

/**
 * The ImageModel is a custom DataModel that provides a list of 10000 images.
 * However the model does not keep all the images in memory all the time but only
 * the subset that is currently needed by the view.
 * For the implementation it utilizes the QContiguousCache class.
 */
class ImageModel : public bb::cascades::DataModel
{
    Q_OBJECT

    /**
     * A property that provides the number of currently cached items. We use it
     * to show that number in the UI.
     */
    Q_PROPERTY(int cachedItemCount READ cachedItemCount NOTIFY cachedItemCountChanged)

public:
    ImageModel(QObject *parent = 0);

    // These methods are re-implemented from the DataModel base class
    int childCount(const QVariantList &indexPath);
    bool hasChildren(const QVariantList &indexPath);
    QString itemType(const QVariantList &indexPath);
    QVariant data(const QVariantList &indexPath);

    // The accessor method for the cached item count property
    int cachedItemCount() const;

Q_SIGNALS:
    // The change notification signal for the cached item count property
    void cachedItemCountChanged();

private:
    // An internal helper method to refill the cache in a given range
    void cacheRows(int from, int to) const;

    // An internal helper method to create a new Image element for the given position
    bb::cascades::Image fetchRow(int position) const;

    // The contiguous cache that caches all the Image objects
    mutable QContiguousCache<bb::cascades::Image> m_rows;

    // The number of images this model provides (10000 by default)
    const int m_count;
};

#endif

