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

#ifndef WORDCOUNTAPP_HPP
#define WORDCOUNTAPP_HPP

#include <QtCore/QObject>
#include <QtCore/QStringList>

/**
 * This sample application shows how to use the QtConcurrent framework
 * to improve the throughput of an algorithm by parallelizing it.
 * The application counts the words inside a list of files, either sequentially
 * or in parallel and shows the elapsed time for that operation.
 */
class WordCountApp : public QObject
{
    Q_OBJECT

    // Makes the number of files that are scanned available to the UI
    Q_PROPERTY(int fileCount READ fileCount NOTIFY fileCountChanged)

    // Makes the number of counted words available to the UI
    Q_PROPERTY(int wordCount READ wordCount NOTIFY wordCountChanged)

    // Makes the time that was needed for counting available to the UI
    Q_PROPERTY(int elapsedTime READ elapsedTime NOTIFY elapsedTimeChanged)

    // Makes the state of the application available to the UI
    Q_PROPERTY(bool active READ active NOTIFY activeChanged)

public:
    WordCountApp();

    // The accessor methods for the properties
    int fileCount() const;
    int wordCount() const;
    int elapsedTime() const;
    bool active() const;

public Q_SLOTS:
    // This methods is invoked when the user clicks the 'Single Threaded' button
    void countSingleThreaded();

    // This methods is invoked when the user clicks the 'Multi Threaded' button
    void countMultiThreaded();

Q_SIGNALS:
    // The change notification signals of the properties
    void fileCountChanged();
    void wordCountChanged();
    void elapsedTimeChanged();
    void activeChanged();

private:
    // A helper method that scans a directory for files matching a given filter
    QStringList findFiles(const QString &startDir, const QStringList &filters);

    // The list of files that are scanned
    QStringList m_files;

    // The number of words that has been counted
    int m_wordCount;

    // The time that was needed for the counting
    int m_elapsedTime;

    // A flag for whether the application is currently counting words
    bool m_active;
};

#endif
