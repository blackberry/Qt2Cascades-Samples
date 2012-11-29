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

#include "xbelreader.hpp"

#include <bb/cascades/QmlDocument>

using namespace bb::cascades;

//! [0]
XbelReader::XbelReader(Container *treeContainer)
    : m_treeContainer(treeContainer)
{
}
//! [0]

//! [1]
bool XbelReader::read(QIODevice *device)
{
    // Set the XBEL document as input data to the XML stream reader
    m_xml.setDevice(device);

    // Read the first element ...
    if (m_xml.readNextStartElement()) {
        // ... and check whether this is a valid XBEL file in the correct version
        if (m_xml.name() == "xbel" && m_xml.attributes().value("version") == "1.0")
            readXBEL(); // Start to parse the document
        else
            m_xml.raiseError(QObject::tr("The file is not an XBEL version 1.0 file."));
    }

    return !m_xml.error();
}
//! [1]

//! [2]
QString XbelReader::errorString() const
{
    return QObject::tr("%1\nLine %2, column %3")
            .arg(m_xml.errorString())
            .arg(m_xml.lineNumber())
            .arg(m_xml.columnNumber());
}
//! [2]

//! [3]
void XbelReader::readXBEL()
{
    // Do sanity check
    Q_ASSERT(m_xml.isStartElement() && m_xml.name() == "xbel");

    // Iterate over the child elements
    while (m_xml.readNextStartElement()) {
        if (m_xml.name() == "folder")
            readFolder(m_treeContainer);
        else if (m_xml.name() == "bookmark")
            readBookmark(m_treeContainer);
        else if (m_xml.name() == "separator")
            readSeparator(m_treeContainer);
        else
            m_xml.skipCurrentElement();
    }
}
//! [3]

//! [4]
void XbelReader::readTitle(Container *item)
{
    // Do sanity check
    Q_ASSERT(m_xml.isStartElement() && m_xml.name() == "title");

    // Read the title property and set it on the control
    const QString title = m_xml.readElementText();
    item->setProperty("title", title);
}
//! [4]

//! [5]
void XbelReader::readSeparator(Container *item)
{
    // Do sanity check
    Q_ASSERT(m_xml.isStartElement() && m_xml.name() == "separator");

    // Just create the control, no additional properties are set
    createChildItem(item);
    m_xml.skipCurrentElement();
}
//! [5]

void XbelReader::readFolder(Container *item)
{
    // Do sanity check
    Q_ASSERT(m_xml.isStartElement() && m_xml.name() == "folder");

    // Create the control for the folder ...
    Container *folder = createChildItem(item);

    // ... and iterate over the fild elements
    while (m_xml.readNextStartElement()) {
        if (m_xml.name() == "title")
            readTitle(folder);
        else if (m_xml.name() == "folder")
            readFolder(folder);
        else if (m_xml.name() == "bookmark")
            readBookmark(folder);
        else if (m_xml.name() == "separator")
            readSeparator(folder);
        else
            m_xml.skipCurrentElement();
    }
}

void XbelReader::readBookmark(Container *item)
{
    // Do sanity check
    Q_ASSERT(m_xml.isStartElement() && m_xml.name() == "bookmark");

    // Create the control for the bookmark and set the properties from the element on it
    Container *bookmark = createChildItem(item);
    bookmark->setProperty("title", QObject::tr("Unknown title"));
    bookmark->setProperty("url", m_xml.attributes().value("href").toString());

    // Read the <title> child element
    while (m_xml.readNextStartElement()) {
        if (m_xml.name() == "title")
            readTitle(bookmark);
        else
            m_xml.skipCurrentElement();
    }
}

Container *XbelReader::createChildItem(Container *parent)
{
    const QString tagName = m_xml.name().toString();

    // Use a different QML file depending on the current XBEL element type
    const QString qmlFile = (tagName == "folder" ? "asset:///FolderItem.qml" :
                             tagName == "bookmark" ? "asset:///BookmarkItem.qml" :
                             tagName == "separator" ? "asset:///SeparatorItem.qml" : QString());

    Container *container = 0;

    // Load the QML file ...
    QmlDocument *qml = QmlDocument::create(qmlFile);
    if (!qml->hasErrors()) {
        // ... create the top-level control ...
        container = qml->createRootObject<Container>();

        // ... and add it to the parent container
        parent->add(container);
    }

    return container;
}
