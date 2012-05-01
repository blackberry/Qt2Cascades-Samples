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

#include "xbelwriter.hpp"

using namespace bb::cascades;

//! [0]
XbelWriter::XbelWriter(Container *treeContainer)
    : m_treeContainer(treeContainer)
{
    // Make the output of the XML stream writer easier to read for humans
    m_xml.setAutoFormatting(true);
}
//! [0]

//! [1]
bool XbelWriter::writeFile(QIODevice *device)
{
    m_xml.setDevice(device);

    // Start the creation of the document
    m_xml.writeStartDocument();
    m_xml.writeDTD("<!DOCTYPE xbel>");

    // Add the root element with the necessary 'xbel' element and version attribute
    m_xml.writeStartElement("xbel");
    m_xml.writeAttribute("version", "1.0");

    // Iterate over the controls of the tree container and generate an XBEL element for each of them
    for (int i = 0; i < m_treeContainer->count(); ++i)
        writeItem(m_treeContainer->at(i));

    m_xml.writeEndDocument();

    return true;
}
//! [1]

//! [2]
void XbelWriter::writeItem(Control *item)
{
    const Container *container = qobject_cast<Container*>(item);

    /**
     * Retrieve the tag name and title from the control.
     * These two properties have been set on the controls by the XbelReader.
     */
    const QString tagName = container->property("tagName").toString();
    const QString title = container->property("title").toString();

    // Depending on the tag name write a new element to the XBEL document
    if (tagName == "folder") {
        m_xml.writeStartElement(tagName);
        m_xml.writeAttribute("folded", "no");
        m_xml.writeTextElement("title", title);

        // Iterate over the child controls of the container
        for (int i = 0; i < container->count(); ++i)
            writeItem(container->at(i));

        m_xml.writeEndElement();
    } else if (tagName == "bookmark") {
        m_xml.writeStartElement(tagName);
        const QString url = container->property("url").toString();
        if (!url.isEmpty())
            m_xml.writeAttribute("href", url);
        m_xml.writeTextElement("title", title);
        m_xml.writeEndElement();
    } else if (tagName == "separator") {
        m_xml.writeEmptyElement(tagName);
    }
}
//! [2]
