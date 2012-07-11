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

#include "ProgressDialogApp.hpp"

using namespace bb::cascades;

/**
 * The 'computation' method that is used to burn some CPU cycles.
 */
//! [0]
void spin(int &iteration)
{
    Q_UNUSED(iteration)

    const int work = 1000 * 1000 * 40;
    volatile int v = 0;
    for (int j = 0; j < work; ++j)
        ++v;
}
//! [0]

//! [1]
ProgressDialogApp::ProgressDialogApp()
    : m_progressMinimum(0)
    , m_progressMaximum(0)
    , m_progressValue(0)
{
    // Here we create a QMLDocument and load it, we are using build patterns.
    QmlDocument *qml = QmlDocument::create().load("main.qml");

    if (!qml->hasErrors()) {
        // Make the ProgressDialogApp object available to the UI as context property
        qml->setContextProperty("_app", this);

        // The application Page is created from QML.
        AbstractPane *appPage = qml->createRootNode<AbstractPane>();

        if (appPage) {
            // Create the application scene.
            Application::instance()->setScene(appPage);
        }
    }

    // Forward state change signals from the watcher to our 'active' property
    connect(&m_futureWatcher, SIGNAL(started()), this, SIGNAL(activeChanged()));
    connect(&m_futureWatcher, SIGNAL(finished()), this, SIGNAL(activeChanged()));

    // Monitor progress changes of the future to update our own properties
    connect(&m_futureWatcher, SIGNAL(progressRangeChanged(int, int)),
            this, SLOT(progressRangeChanged(int, int)));
    connect(&m_futureWatcher, SIGNAL(progressValueChanged(int)),
            this, SLOT(progressValueChanged(int)));
    connect(&m_futureWatcher, SIGNAL(finished()),
            this, SLOT(calculationFinished()));
}
//! [1]

//! [2]
int ProgressDialogApp::numberOfCores() const
{
    /**
     * The QtConcurrent framework uses QThreadPool::globalInstance()
     * for computation and that one uses as many thread contexts as
     * returned by QThread::idelThreadCount().
     */
    return QThread::idealThreadCount();
}
//! [2]

bool ProgressDialogApp::active() const
{
    // Return whether there is currently an active Future
    return m_futureWatcher.isRunning();
}

int ProgressDialogApp::progressMinimum() const
{
    return m_progressMinimum;
}

int ProgressDialogApp::progressMaximum() const
{
    return m_progressMaximum;
}

int ProgressDialogApp::progressValue() const
{
    return m_progressValue;
}

//! [3]
void ProgressDialogApp::startComputation()
{
    // Prepare the vector
    QVector<int> vector;
    for (int i = 0; i < 40; ++i)
        vector.append(i);

    /**
     * Start the computation.
     * The returned Future object is just a handle to the running operation, which
     * is executed asynchronously in separated threads.
     */
    const QFuture<void> future = QtConcurrent::map(vector, spin);

    // Let the future watcher monitor the progress of this Future
    m_futureWatcher.setFuture(future);
}
//! [3]

//! [4]
void ProgressDialogApp::cancelComputation()
{
    // Stop the computation
    m_futureWatcher.cancel();
}
//! [4]

void ProgressDialogApp::progressRangeChanged(int minimum, int maximum)
{
    m_progressMinimum = minimum;
    m_progressMaximum = maximum;

    emit progressRangeChanged();
}

void ProgressDialogApp::progressValueChanged(int value)
{
    if (m_progressValue == value)
        return;

    m_progressValue = value;
    emit progressValueChanged();
}

void ProgressDialogApp::calculationFinished()
{
    // Reset the progress indicator after calculation has finished or was canceled
    m_progressValue = 0;
    emit progressValueChanged();
}
