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

#include <bb/cascades/Application>
#include <bb/cascades/Control>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Page>

#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QTime>
#include <QtCore/QWaitCondition>

using namespace bb::cascades;

//! [0]
// The total number of bytes that will be produced and consumed
static const int DataSize = 100000;

// The size of the buffer that is used to deliver data from the producer to the consumer
static const int BufferSize = 1900;

// The buffer that is used to deliver data from the producer to the consumer
static char buffer[BufferSize];

// The wait condition that represents the state of an not-empty buffer
static QWaitCondition bufferNotEmpty;

// The wait condition that represents the state of an not-full buffer
static QWaitCondition bufferNotFull;

// The mutex that synchronizes the usage of the wait conditions
static QMutex mutex;

// The number of bytes that are used inside the shared buffer
static int numUsedBytes = 0;

//! [0]

//! [1]
/**
 * The Producer fills the shared buffer with data in a separated worker thread.
 */
class Producer : public QThread
//! [1] //! [2]
{
public:
    void run()
    {
        // This code is executed in the worker thread

        // Initialize the random number generator
        qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

        for (int i = 0; i < DataSize; ++i) {
            mutex.lock();
            // Check whether we have written so many bytes that the shared buffer is completely filled.
            if (numUsedBytes == BufferSize) {
                /**
                 * In this case wait on the 'bufferNotFull' condition.
                 * This will block us until the Consumer calls wakeAll() on this wait condition.
                 */
                bufferNotFull.wait(&mutex);
            }
            mutex.unlock();

            // Write on byte to the shared buffer ...
            buffer[i % BufferSize] = "ACGT"[(int) qrand() % 4];

            mutex.lock();
            // ... increment the number of used bytes ...
            ++numUsedBytes;

            // ... and wake up the Consumer if it has been blocked.
            bufferNotEmpty.wakeAll();
            mutex.unlock();
        }
    }
};
//! [2]

//! [3]
/**
 * The Consumer reads the data from the shared buffer in a separated worker thread and
 * publishes the read data to the UI.
 */
class Consumer : public QThread
//! [3] //! [4]
{
    Q_OBJECT

public:
    void run()
    {
        for (int i = 0; i < DataSize; ++i) {
            mutex.lock();
            // Check whether we have read so many bytes that the shared buffer is empty.
            if (numUsedBytes == 0) {
                /**
                 * In this case wait on the 'bufferNotEmpty' condition.
                 * This will block us until the Producer calls wakeAll() on this wait condition.
                 */
                bufferNotEmpty.wait(&mutex);
            }
            mutex.unlock();

            // Read on byte from the shared buffer ...
            const QString text(buffer[i % BufferSize]);

            mutex.lock();
            // ... decrement the number of used bytes ...
            --numUsedBytes;

            // ... and wake up the Producer if it has been blocked.
            bufferNotFull.wakeAll();
            mutex.unlock();

            // Publish the result to the UI
            emit stringConsumed(text);
        }

        // Tell the UI that we have transferred all data
        emit stringConsumed("\n\nFinished");
    }

Q_SIGNALS:
    void stringConsumed(const QString &text);
};
//! [4]

/**
 * The TextBuffer is a helper class that collects the data emitted by the Consumer
 * and makes them available to the UI via the 'text' property.
 */
class TextBuffer : public QObject
{
    Q_OBJECT

    // Makes the consumed data available to the UI
    Q_PROPERTY(QString text READ text NOTIFY textChanged)

public:
    explicit TextBuffer(QObject *parent = 0);

    // The accessor method of the text property
    QString text() const;

public Q_SLOTS:
    // This method is invoked whenever the Consumer reports new data
    void appendText(const QString &text);

Q_SIGNALS:
    // The change notification signal of the text property
    void textChanged();

private:
    // The text property
    QString m_text;
};

TextBuffer::TextBuffer(QObject *parent)
    : QObject(parent)
{
}

QString TextBuffer::text() const
{
    return m_text;
}

void TextBuffer::appendText(const QString &text)
{
    // Append the new data from the Consumer to our internal buffer
    m_text += text;

    // Limit the text size, too much text makes TextArea slow
    if (m_text.length() > BufferSize) {
        m_text.clear();
    }

    // Signal the UI that the data have changed
    emit textChanged();
}

//! [5]
/**
 * This sample application shows how to solve the producer-consumer problem (http://en.wikipedia.org/wiki/Producer-consumer_problem)
 * with wait conditions under Qt.
 */
int main(int argc, char **argv)
//! [5] //! [6]
{
    Application app(argc, argv);

    // Create the producer and consumer objects
    Producer producer;
    Consumer consumer;

    // Create the TextBuffer object
    TextBuffer textBuffer;

    // Append the new data to the TextBuffer whenever the Consumer has consumed some data
    QObject::connect(&consumer, SIGNAL(stringConsumed(const QString&)),
                     &textBuffer, SLOT(appendText(const QString&)), Qt::BlockingQueuedConnection);

    QmlDocument * const qml = QmlDocument::create().load("main.qml");
    if (!qml->hasErrors()) {
        // Make the TextBuffer available to the UI as context property
        qml->setContextProperty("_textBuffer", &textBuffer);
        Page *appPage = qml->createRootNode<Page>();
        if (appPage) {
            Application::instance()->setScene(appPage);

            // Start the producer and consumer thread
            producer.start();
            consumer.start();
        }
    }
    return Application::exec();
}
//! [6]

#include "main.moc"
