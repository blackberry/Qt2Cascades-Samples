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

#ifndef TRAFFICLIGHTCONTROLLER_HPP
#define TRAFFICLIGHTCONTROLLER_HPP

#include <QObject>

class QTimer;

/**
 * TrafficLightController
 *
 * The class that contains the business logic of the application.
 * It represents a traffic light that automatically switches between the three
 * colors red, yellow and green.
 * The control flow is orchestrated by a QStateMachine, which can be paused in
 * execution.
 * The states of the single lights of the traffic light are made available via
 * properties, so that the UI can bind against them directly.
 */
//! [0]
class TrafficLightController : public QObject
{
    Q_OBJECT

    // the state properties for the single lights
    Q_PROPERTY(bool redOn READ redOn WRITE setRedOn NOTIFY redOnChanged)
    Q_PROPERTY(bool yellowOn READ yellowOn WRITE setYellowOn NOTIFY yellowOnChanged)
    Q_PROPERTY(bool greenOn READ greenOn WRITE setGreenOn NOTIFY greenOnChanged)

    // the state property that reflects whether the traffic light is currently paused
    Q_PROPERTY(bool paused READ paused WRITE setPaused NOTIFY pausedChanged)
//! [0]

public:
    TrafficLightController(QObject *parent = 0);

    // accessor methods for the state properties
    bool redOn() const;
    void setRedOn(bool on);

    bool yellowOn() const;
    void setYellowOn(bool on);

    bool greenOn() const;
    void setGreenOn(bool on);

    bool paused() const;
    void setPaused(bool paused);

public Q_SLOTS:
    /**
     * This slot is called by the UI to pause/continue the traffic light
     */
    void pause();

Q_SIGNALS:
    // change notification signals for the state properties
    void redOnChanged();
    void yellowOnChanged();
    void greenOnChanged();
    void pausedChanged();

    /**
     * This signal is emitted when the pause() slot is invoked and is used
     * by the internal state machine only.
     */
    void pauseRequested();

private:
    /**
     * Creates the internal state machine.
     */
    void setupStateMachine();

    // state property variables
    bool m_redOn;
    bool m_yellowOn;
    bool m_greenOn;
    bool m_paused;

    // the timer that triggers the state changes
    QTimer *m_timer;
};

#endif
