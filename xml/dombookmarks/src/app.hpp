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

#ifndef APP_HPP
#define APP_HPP

#include <QtCore/QObject>
#include <QtCore/QMetaType>

#include <bb/cascades/Container>
#include <bb/cascades/Event>
#include <bb/cascades/UiObject>

/**
 * This sample application shows how to read and write XML documents with
 * the DOM API provided by Qt.
 *
 * The user can load one of two predefined files in the XBEL (XML Bookmark specification) format
 * and the program will show the bookmarks on the screen in form of a tree.
 * The user can modify the links of the bookmarks now and save them again.
 */
class App: public QObject
{
    Q_OBJECT

    // Makes the status of the application available to the UI
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)

public:
    App();

    // This method is invoked when the user wants to load an XBEL file
    Q_INVOKABLE void load(const QString &fileName);

    // This method is invoked when the user wants to save the currently loaded XBEL file
    Q_INVOKABLE void save();

Q_SIGNALS:
    // The change notification signal of the status property
    void statusChanged();

private:
    // The accessor method of the status property
    QString status() const;

    // The Control where the bookmark tree is visualized in
    QPointer<bb::cascades::Container> m_treeContainer;

    // A textual description of the application status
    QString m_status;
};

#endif
