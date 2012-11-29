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

#include "xmlstreamlint.hpp"

#include <QtCore/QBuffer>
#include <QtCore/QFile>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QXmlStreamWriter>

XmlStreamLint::XmlStreamLint(QObject *parent)
    : QObject(parent)
{
}

void XmlStreamLint::checkXmlFile(const QString &fileName)
{
    // Initialize the properties with some sane values
    m_result = tr("No syntax errors");
    emit resultChanged();

    m_output.clear();
    emit outputChanged();

    // Try to open the passed file and report an error if that doesn't work
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        m_result = tr("Couldn't open the file.");
        emit resultChanged();
        return;
    }

    // Create a memory buffer where we save the validated and reformatted XML document
    QByteArray outputData;
    QBuffer buffer(&outputData);
    buffer.open(QIODevice::WriteOnly);

//! [0]
    // Create a XML stream reader that reads the XML document from the file
    QXmlStreamReader reader(&file);

    // Create a XML stream reader that writes an XML document to the memory buffer
    QXmlStreamWriter writer(&buffer);
//! [0]

//! [1]
    // Now let the XML stream reader parse the XML document token by token ...
    while (!reader.atEnd()) {
        reader.readNext();

        // ... and update the result property if an error occurs ...
        if (reader.error()) {
            m_result = tr("Error: %1 in file %2 at line %3, column %4.\n")
                         .arg(reader.errorString(), fileName, QString::number(reader.lineNumber()), QString::number(reader.columnNumber()));
            emit resultChanged();
//! [1]

//! [2]
        } else {
            // ... otherwise let the XML stream writer write out this token
            writer.writeCurrentToken(reader);
        }
    }
//! [2]

    // Update the output property with the content of the memory buffer
    m_output = QString::fromUtf8(outputData);
    emit outputChanged();
}

QString XmlStreamLint::result() const
{
    return m_result;
}

QString XmlStreamLint::output() const
{
    return m_output;
}
