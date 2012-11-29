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

#ifndef XBELPARSER_HPP
#define XBELPARSER_HPP

#include <bb/cascades/Container>

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

//! [0]
/**
 * The XbelParser is responsible for parsing a XBEL file and generating
 * controls, that represent the bookmark entries, on the tree container.
 *
 * To parse the XBEL document the XbelParser uses the DOM API of Qt.
 */
class XbelParser
{
public:
    XbelParser(bb::cascades::Container *treeContainer);

    // Starts the parsing of the XBEL document
    bool parse(QIODevice *device);

    // Returns a textual representation of the error if one occurred
    QString errorString() const;

private:
    // A helper method that parses one XBEL folder element
    void parseFolderElement(const QDomElement &element, bb::cascades::Container *parent = 0);

    // A helper method that generates a control for a specific XBEL element
    bb::cascades::Container *createChildItem(const QDomElement &element, bb::cascades::Container *parent = 0);

    // The container object the controls are created in
    QPointer<bb::cascades::Container> m_treeContainer;

    // The textual representation of an error
    QString m_errorString;
};
//! [0]

#endif
