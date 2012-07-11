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

#include "xbelparser.hpp"

#include <bb/cascades/QmlDocument>

using namespace bb::cascades;

//! [0]
XbelParser::XbelParser(Container *treeContainer)
    : m_treeContainer(treeContainer)
{
}
//! [0]

//! [1]
bool XbelParser::parse(QIODevice *device)
{
    // Open a DOM document on the XBEL data and report an error if it fails
    QString errorString;
    int errorLine;
    int errorColumn;

    QDomDocument domDocument;
    if (!domDocument.setContent(device, true, &errorString, &errorLine, &errorColumn)) {
        m_errorString = QObject::tr("Parse error at line %1, column %2:\n%3").arg(errorLine)
                .arg(errorColumn)
                .arg(errorString);
        return false;
    }

    // Check whether this is a valid XBEL file in the correct version
    const QDomElement rootElement = domDocument.documentElement();
    if (rootElement.tagName() != "xbel") {
        m_errorString = QObject::tr("The file is not an XBEL file.");
        return false;
    } else if (rootElement.hasAttribute("version") && rootElement.attribute("version") != "1.0") {
        m_errorString = QObject::tr("The file is not an XBEL version 1.0 file");
        return false;
    }

    // Search for the first <folder> element ...
    QDomElement child = rootElement.firstChildElement("folder");
    while (!child.isNull()) {
        // ... parse this folder element ...
        parseFolderElement(child);

        // ... and search for the next one until the end of the XBEL document is reached
        child = child.nextSiblingElement("folder");
    }

    return true;
}
//! [1]

//! [2]
QString XbelParser::errorString() const
{
    return m_errorString;
}
//! [2]

//! [3]
void XbelParser::parseFolderElement(const QDomElement &element, Container *parent)
{
    // Create a container that will contain all the child controls from this <folder> element
    Container *item = createChildItem(element, parent);

    // Read the title from the DOM document ...
    QString title = element.firstChildElement("title").text();
    if (title.isEmpty())
        title = QObject::tr("Folder");

    // ... and set it as 'title' property on the container
    item->setProperty("title", title);

    // Iterate over the children of the <folder> element, read the properties and set them on the control
    QDomElement child = element.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == "folder") {
            parseFolderElement(child, item);
        } else if (child.tagName() == "bookmark") {
            Container *childItem = createChildItem(child, item);

            QString title = child.firstChildElement("title").text();
            if (title.isEmpty())
                title = QObject::tr("Folder");

            childItem->setProperty("title", title);
            childItem->setProperty("url", child.attribute("href"));
        } else if (child.tagName() == "separator") {
            createChildItem(child, item);
        }
        child = child.nextSiblingElement();
    }
}
//! [3]

//! [4]
Container *XbelParser::createChildItem(const QDomElement &element, Container *parent)
{
    const QString tagName = element.tagName();

    // Use a different QML file depending on the requested XBEL element type
    const QString qmlFile = (tagName == "folder" ? "FolderItem.qml" :
                             tagName == "bookmark" ? "BookmarkItem.qml" :
                             tagName == "separator" ? "SeparatorItem.qml" : QString());

    Container *container = 0;

    // Load the QML file ...
    QmlDocument *qml = QmlDocument::create().load(qmlFile);
    if (!qml->hasErrors()) {
        // ... create the top-level control ...
        container = qml->createRootNode<Container>();

        // ... and add it to the parent container
        if (parent) {
            parent->add(container);
        } else {
            m_treeContainer->add(container);
        }
    }

    return container;
}
//! [4]
