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

#ifndef CODECCONTROLLER_HPP
#define CODECCONTROLLER_HPP

#include <bb/cascades/DropDown>

#include <QtCore/QObject>
#include <QtCore/QPointer>

/**
 * The CodecController class contains all the business logic of this application.
 * It fills the file and codec DropDown lists (which are set from within the UI)
 * and updated the decoded file content that will be shown in the UI.
 */
class CodecController : public QObject
{
    Q_OBJECT

    /**
     * These two properties are used to define the file and codec DropDown lists from within the UI
     */
    Q_PROPERTY(bb::cascades::DropDown* fileList READ fileList WRITE setFileList NOTIFY fileListChanged)
    Q_PROPERTY(bb::cascades::DropDown* codecList READ codecList WRITE setCodecList NOTIFY codecListChanged)

    // The property that makes the decoded file content available to the UI
    Q_PROPERTY(QString fileContent READ fileContent NOTIFY fileContentChanged)

public:
    CodecController(QObject *parent = 0);

    // The accessor methods for the properties
    void setFileList(bb::cascades::DropDown *fileList);
    bb::cascades::DropDown* fileList() const;

    void setCodecList(bb::cascades::DropDown *codecList);
    bb::cascades::DropDown* codecList() const;

    QString fileContent() const;

public Q_SLOTS:
    // This method is called whenever the user changes the selection in the file or codec list
    void updateFileContent();

Q_SIGNALS:
    // The change notification signals for the properties
    void fileListChanged();
    void codecListChanged();
    void fileContentChanged();

private:
    // This method fills the file DropDown with the available example files
    void fillFileList();

    // This method fills the codec DropDown with available codecs of the system
    void fillCodecList();

    // The DropDown object for the file list
    QPointer<bb::cascades::DropDown> m_fileList;

    // The DropDown object for the codec list
    QPointer<bb::cascades::DropDown> m_codecList;

    // The decoded content of the currently selected file
    QString m_fileContent;
};

#endif
