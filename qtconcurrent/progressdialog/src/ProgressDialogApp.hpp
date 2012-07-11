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

#ifndef PROGRESSDIALOGAPP_HPP
#define PROGRESSDIALOGAPP_HPP

#include <QtCore/QFutureWatcher>
#include <QtCore/QObject>

/**
 * This sample application shows how to use Future objects from QtConcurrent
 * to monitor and change the state of a running computation.
 * The application provides an UI where the user can start an computation that
 * is distributed over all available CPU cores by the QtConcurrent framework.
 * A progress bar shows the current progress of the computation and the user
 * can cancel the computation at any point in time.
 */
class ProgressDialogApp : public QObject
{
    Q_OBJECT

    // Makes the number of available CPU cores available to the UI
    Q_PROPERTY(int numberOfCores READ numberOfCores NOTIFY numberOfCoresChanged)

    // Makes the current state of the application available to the UI
    Q_PROPERTY(bool active READ active NOTIFY activeChanged);

    // Makes the minimum of the progress indicator available to the UI
    Q_PROPERTY(int progressMinimum READ progressMinimum NOTIFY progressRangeChanged)

    // Makes the maximum of the progress indicator available to the UI
    Q_PROPERTY(int progressMaximum READ progressMaximum NOTIFY progressRangeChanged)

    // Makes the current progress value available to the UI
    Q_PROPERTY(int progressValue READ progressValue NOTIFY progressValueChanged)

public:
    ProgressDialogApp();

    // The accessor methods for the properties
    int numberOfCores() const;
    bool active() const;
    int progressMinimum() const;
    int progressMaximum() const;
    int progressValue() const;

public Q_SLOTS:
    // This method is invoked when the user clicks on the 'Start' button in the UI
    void startComputation();

    // This method is invoked when the user clicks the 'Cancel' button in the UI
    void cancelComputation();

Q_SIGNALS:
    // The change notification signals of the properties
    void numberOfCoresChanged();
    void activeChanged();
    void progressRangeChanged();
    void progressValueChanged();

private Q_SLOTS:
    // This slot is invoked whenever the future reports a change of the progress range
    void progressRangeChanged(int minimum, int maximum);

    // This slot is invoked whenever the future reports a change of the current progress value
    void progressValueChanged(int value);

    // This slot is invoked whenever the future reports to be finished
    void calculationFinished();

private:
    // The future watcher that provides monitoring for the currently running computation
    QFutureWatcher<void> m_futureWatcher;

    // The minimum progress value
    int m_progressMinimum;

    // The maximum progress value
    int m_progressMaximum;

    // The current progress value
    int m_progressValue;
};

#endif
