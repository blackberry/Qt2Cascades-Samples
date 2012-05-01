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
    // Set the output device on the stream
    m_stream.setDevice(device);
    m_stream.setCodec("UTF-8");

    // Add the root element with the necessary 'xbel' element and version attribute
    m_stream << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
             << "<!DOCTYPE xbel>\n"
             << "<xbel version=\"1.0\">\n";

    // Iterate over the controls of the tree container and generate an XBEL element for each of them
    for (int i = 0; i < m_treeContainer->count(); ++i)
        generateItem(m_treeContainer->at(i), 1);

    // Finish the XBEL document
    m_stream << "</xbel>\n";

    return true;
}

QString XbelGenerator::indent(int depth)
{
    const int IndentSize = 4;
    return QString(IndentSize * depth, ' ');
}

QString XbelGenerator::escapedText(const QString &str)
{
    QString result = str;
    result.replace("&", "&amp;");
    result.replace("<", "&lt;");
    result.replace(">", "&gt;");
    return result;
}

QString XbelGenerator::escapedAttribute(const QString &str)
{
    QString result = escapedText(str);
    result.replace("\"", "&quot;");
    result.prepend("\"");
    result.append("\"");
    return result;
}

void XbelGenerator::generateItem(Control *control, int depth)
{
    /**
     * Retrieve the tag name and title from the control.
     * These two properties have been set on the controls by the XbelHandler.
     */
    const QString tagName = control->property("tagName").toString();
    const QString title = control->property("title").toString();

    // Depending on the tag name write a new element to the XBEL document
    if (tagName == "folder") {
        m_stream << indent(depth) << "<folder folded=\"no\">\n"
                 << indent(depth + 1) << "<title>" << escapedText(title)
                 << "</title>\n";

        // Iterate over the child controls of the container
        const Container *container = qobject_cast<Container*>(control);
        for (int i = 0; i < container->count(); ++i)
            generateItem(container->at(i), depth + 1);

        m_stream << indent(depth) << "</folder>\n";
    } else if (tagName == "bookmark") {
        const QString url = control->property("url").toString();

        m_stream << indent(depth) << "<bookmark";
        if (!url.isEmpty())
            m_stream << " href=" << escapedAttribute(url);

        m_stream << ">\n"
                 << indent(depth + 1) << "<title>" << escapedText(title)
                 << "</title>\n"
                 << indent(depth) << "</bookmark>\n";
    } else if (tagName == "separator") {
        m_stream << indent(depth) << "<separator/>\n";
    }
}
