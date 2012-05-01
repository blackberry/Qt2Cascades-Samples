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

#include "xbelgenerator.hpp"

using namespace bb::cascades;

XbelGenerator::XbelGenerator(Container *treeContainer)
        : m_treeContainer(treeContainer)
{
}

bool XbelGenerator::write(QIODevice *device)
{
    // Create a new DOM document
    QDomDocument domDocument("xbel");
    domDocument.appendChild(domDocument.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"utf-8\""));

    // Add the root element with the necessary 'xbel' element and version attribute
    QDomElement rootElement = domDocument.createElement("xbel");
    rootElement.setAttribute("version", "1.0");
    domDocument.appendChild(rootElement);

    // Iterate over the controls of the tree container and generate an XBEL element for each of them
    for (int i = 0; i < m_treeContainer->count(); ++i)
        generateItem(m_treeContainer->at(i), rootElement, domDocument);

    const int IndentWidth = 4;

    // Use a text stream to write out the DOM document in a nicely formatted way
    QTextStream stream(device);
    stream.setCodec("UTF-8");
    domDocument.save(stream, IndentWidth);

    return true;
}

void XbelGenerator::generateItem(Control *control, QDomElement &parent, QDomDocument &domDocument)
{
    /**
     * Retrieve the tag name and title from the control.
     * These two properties have been set on the controls by the XbelParser.
     */
    const QString tagName = control->property("tagName").toString();
    const QString title = control->property("title").toString();

    // Depending on the tag name generate a new DOM element in the XBEL document
    if (tagName == "folder") {
        QDomElement folderElement = domDocument.createElement("folder");
        folderElement.setAttribute("folded", "no");

        QDomElement titleElement = domDocument.createElement("title");
        QDomText titleText = domDocument.createTextNode(title);
        titleElement.appendChild(titleText);
        folderElement.appendChild(titleElement);

        // Iterate over the child controls of the container
        const Container *container = qobject_cast<Container*>(control);
        for (int i = 0; i < container->count(); ++i)
            generateItem(container->at(i), folderElement, domDocument);

        parent.appendChild(folderElement);
    } else if (tagName == "bookmark") {
        const QString url = control->property("url").toString();

        QDomElement bookmarkElement = domDocument.createElement("bookmark");
        if (!url.isEmpty())
            bookmarkElement.setAttribute("href", url);

        QDomElement titleElement = domDocument.createElement("title");
        QDomText titleText = domDocument.createTextNode(title);
        titleElement.appendChild(titleText);
        bookmarkElement.appendChild(titleElement);

        parent.appendChild(bookmarkElement);
    } else if (tagName == "separator") {
        QDomElement separatorElement = domDocument.createElement("separator");
        parent.appendChild(separatorElement);
    }
}
