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

#include "xbelhandler.hpp"

#include <bb/cascades/QmlDocument>

using namespace bb::cascades;

//! [0]
XbelHandler::XbelHandler(Container *treeContainer)
    : m_treeContainer(treeContainer)
{
    m_item = 0;
    m_metXbelTag = false;
}
//! [0]

//! [1]
bool XbelHandler::startElement(const QString & /* namespaceURI */,
                               const QString & /* localName */,
                               const QString &qName,
                               const QXmlAttributes &attributes)
{
    // The first element must be the 'xbel' element
    if (!m_metXbelTag && qName != "xbel") {
        m_errorStr = QObject::tr("The file is not an XBEL file.");
        return false;
    }

    // Read the properties from the element and store them in the control
    if (qName == "xbel") {
        const QString version = attributes.value("version");
        if (!version.isEmpty() && version != "1.0") {
            m_errorStr = QObject::tr("The file is not an XBEL version 1.0 file.");
            return false;
        }
        m_metXbelTag = true;
    } else if (qName == "folder") {
        m_item = createChildItem(qName);
        m_item->setProperty("title", QObject::tr("Folder"));
    } else if (qName == "bookmark") {
        m_item = createChildItem(qName);
        m_item->setProperty("title", QObject::tr("Unknown title"));
        m_item->setProperty("url", attributes.value("href"));
    } else if (qName == "separator") {
        m_item = createChildItem(qName);
    }

    m_currentText.clear();
    return true;
}
//! [1]

//! [2]
bool XbelHandler::endElement(const QString & /* namespaceURI */,
                             const QString & /* localName */,
                             const QString &qName)
{
    if (qName == "title") {
        if (m_item)
            m_item->setProperty("title", m_currentText);
    } else if (qName == "folder" || qName == "bookmark" || qName == "separator") {
        m_item = qobject_cast<Container*>(m_item->parent());
    }
    return true;
}
//! [2]

bool XbelHandler::characters(const QString &str)
{
    // Collect all characters so that we can use them in endElement() after the </title> tag
    m_currentText += str;
    return true;
}

bool XbelHandler::fatalError(const QXmlParseException &exception)
{
    qWarning() << QObject::tr("Parse error at line %1, column %2:\n%3")
                             .arg(exception.lineNumber())
                             .arg(exception.columnNumber())
                             .arg(exception.message());
    return false;
}

//! [3]
QString XbelHandler::errorString() const
{
    return m_errorStr;
}
//! [3]

//! [4]
Container *XbelHandler::createChildItem(const QString &tagName)
{
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
        if (m_item) {
            m_item->add(container);
        } else {
            m_treeContainer->add(container);
        }
    }

    return container;
}
//! [4]
