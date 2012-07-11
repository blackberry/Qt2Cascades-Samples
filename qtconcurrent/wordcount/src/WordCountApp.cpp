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

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>

#include "WordCountApp.hpp"

#include <numeric> // for std::accumulate

#include <qtconcurrentmap.h>

using namespace bb::cascades;

/**
 * A helper structure that maps the name of a file
 * to the number of words it contains.
 */
//! [0]
typedef QMap<QString, int> WordCount;
//! [0]

/**
 * A function that counts words single threaded
 * by iterating over the files sequentially.
 */
//! [1]
WordCount singleThreadedWordCount(const QStringList &files)
{
    WordCount wordCount;

    foreach (const QString &fileName, files) {
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        QTextStream textStream(&file);
        while (textStream.atEnd() == false) {
            foreach(const QString &word, textStream.readLine().split(" "))
                wordCount[word] += 1;
        }
    }

    return wordCount;
}
//! [1]

/**
 * A function that counts the words in a single file.
 * This function is called in parallel by several threads, therefor it
 * must be thread safe.
 */
//! [2]
WordCount countWords(const QString &fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QTextStream textStream(&file);
    WordCount wordCount;

    while (textStream.atEnd() == false) {
        foreach (const QString &word, textStream.readLine().split(" "))
            wordCount[word] += 1;
    }

    return wordCount;
}
//! [2]

/**
 * A function that adds the results from the map to the final
 * result. This functor will only be called by one thread at a time.
 */
//! [3]
void reduce(WordCount &result, const WordCount &w)
{
    QMapIterator<QString, int> i(w);
    while (i.hasNext()) {
        i.next();
        result[i.key()] += i.value();
    }
}
//! [3]


WordCountApp::WordCountApp()
    : m_wordCount(0), m_elapsedTime(0), m_active(false)
{
    // Here we create a QMLDocument and load it, we are using build patterns.
    QmlDocument *qml = QmlDocument::create().load("main.qml");

    if (!qml->hasErrors()) {
        // Make the WordCountApp object available to the UI as context property
        qml->setContextProperty("_app", this);

        // The application Page is created from QML.
        AbstractPane *appPage = qml->createRootNode<AbstractPane>();

        if (appPage) {
            // Create the application scene.
            Application::instance()->setScene(appPage);
        }
    }

    // Find a couple of files in the file system
    m_files = findFiles("/usr/photon/", QStringList() << "*");
    m_files += findFiles("/scripts/", QStringList() << "*");

    emit fileCountChanged();
}

int WordCountApp::fileCount() const
{
    return m_files.count();
}

int WordCountApp::wordCount() const
{
    return m_wordCount;
}

int WordCountApp::elapsedTime() const
{
    return m_elapsedTime;
}

bool WordCountApp::active() const
{
    return m_active;
}

//! [4]
void WordCountApp::countSingleThreaded()
{
    // Mark the application as active
    m_active = true;
    emit activeChanged();

    // Start to measure the time
    QTime time;
    time.start();

    // Count the words single threaded
    const WordCount total = singleThreadedWordCount(m_files);

    // Update the measured time
    m_elapsedTime = time.elapsed();
    emit elapsedTimeChanged();

    // Accumulate the per-file word counts to the total word count
    const QList<int> counts = total.values();
    m_wordCount = std::accumulate(counts.begin(), counts.end(), 0);
    emit wordCountChanged();

    // Mark the application as inactive
    m_active = false;
    emit activeChanged();
}
//! [4]

//! [5]
void WordCountApp::countMultiThreaded()
{
    // Mark the application as active
    m_active = true;
    emit activeChanged();

    // Start to measure the time
    QTime time;
    time.start();

    // Count the words multi-threaded by using QtConcurrent
    const WordCount total = QtConcurrent::mappedReduced(m_files, countWords, reduce);

    // Update the measured time
    m_elapsedTime = time.elapsed();
    emit elapsedTimeChanged();

    // Accumulate the per-file word counts to the total word count
    const QList<int> counts = total.values();
    m_wordCount = std::accumulate(counts.begin(), counts.end(), 0);
    emit wordCountChanged();

    // Mark the application as inactive
    m_active = false;
    emit activeChanged();
}
//! [5]

QStringList WordCountApp::findFiles(const QString &startDir, const QStringList &filters)
{
    QStringList names;
    QDir dir(startDir);

    // Collect all files from local dir that match the filter
    foreach (const QString &file, dir.entryList(filters, QDir::Files))
        names += startDir + "/" + file;

    // Iterate over sub-directories
    foreach (const QString &subdir, dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot))
        names += findFiles(startDir + "/" + subdir, filters);

    return names;
}
