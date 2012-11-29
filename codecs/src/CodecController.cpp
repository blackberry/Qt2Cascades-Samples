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

#include "CodecController.hpp"

using namespace bb::cascades;

Q_DECLARE_METATYPE(QTextCodec*)

CodecController::CodecController(QObject *parent)
    : QObject(parent)
    , m_fileList(0)
    , m_codecList(0)
{
}

void CodecController::setFileList(DropDown *fileList)
{
    if (m_fileList != fileList) {
        m_fileList = fileList;

        fillFileList();

        emit fileListChanged();
    }
}

DropDown* CodecController::fileList() const
{
    return m_fileList;
}

void CodecController::setCodecList(DropDown *codecList)
{
    if (m_codecList != codecList) {
        m_codecList = codecList;

        fillCodecList();

        emit codecListChanged();
    }
}

DropDown* CodecController::codecList() const
{
    return m_codecList;
}

QString CodecController::fileContent() const
{
    return m_fileContent;
}

//! [0]
void CodecController::updateFileContent()
{
    if (!m_fileList || !m_codecList)
        return;

    // Check whether the user has selected a file already
    const int selectedIndex = m_fileList->selectedIndex();
    if (selectedIndex == DropDown::SelectedIndexNone)
        return;

    // Retrieve the selected Options from the file and codec DropDowns
    const Option *selectedFileOption = m_fileList->at(selectedIndex);
    const Option *selectedCodecOption = m_codecList->at(m_codecList->selectedIndex());

    // Extract the values associated with the selected Options
    const QString fileName = selectedFileOption->value().toString();
    QTextCodec *codec = selectedCodecOption->value().value<QTextCodec*>();

    // Open the selected file ...
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return;

    // ... set a QTextStream with the selected codec on it ...
    QTextStream stream(&file);
    stream.setAutoDetectUnicode(false);
    stream.setCodec(codec);

    // ... and read out the decoded content
    m_fileContent = stream.readAll();

    emit fileContentChanged();
}
//! [0]

//! [1]
void CodecController::fillFileList()
{
    if (!m_fileList)
        return;

    const QStringList fileNames = QStringList()
        << "iso-8859-1.txt"
        << "iso-8859-15.txt"
        << "utf-16.txt"
        << "utf-16be.txt"
        << "utf-16le.txt"
        << "utf-8.txt";

    // Clear the file DropDown
    m_fileList->removeAll();

    // Create one entry for each file
    foreach (const QString fileName, fileNames) {
        Option *option = new Option;
        option->setText(fileName);
        option->setValue(QString("app/native/assets/encodedfiles/%1").arg(fileName));
        m_fileList->add(option);
    }
}
//! [1]

//! [2]
void CodecController::fillCodecList()
{
    if (!m_codecList)
        return;

    // Clear the codec DropDown
    m_codecList->removeAll();

    // Create one entry for each codec available on the system
    foreach (int mib, QTextCodec::availableMibs()) {
        QTextCodec *codec = QTextCodec::codecForMib(mib);

        Option *option = new Option;
        option->setText(codec->name());
        option->setValue(QVariant::fromValue(codec));

        // pre-select the UTF-8 codec in the DropDown
        if (codec->name() == "UTF-8")
            option->setSelected(true);

        m_codecList->add(option);
    }
}
//! [2]
