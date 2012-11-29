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

#include "TrafficLightController.hpp"

#include <QHistoryState>
#include <QStateMachine>
#include <QTimer>

//! [0]
TrafficLightController::TrafficLightController(QObject *parent)
    : QObject(parent)
    , m_redOn(false)
    , m_yellowOn(false)
    , m_greenOn(false)
    , m_paused(false)
    , m_timer(new QTimer(this))
{
    // Create and start the state machine
    setupStateMachine();

    // Start the timer to trigger state changes
    m_timer->start();
}
//! [0]

bool TrafficLightController::redOn() const
{
    return m_redOn;
}

void TrafficLightController::setRedOn(bool on)
{
    if (m_redOn != on) {
        m_redOn = on;
        emit redOnChanged();
    }
}

bool TrafficLightController::yellowOn() const
{
    return m_yellowOn;
}

void TrafficLightController::setYellowOn(bool on)
{
    if (m_yellowOn != on) {
        m_yellowOn = on;
        emit yellowOnChanged();
    }
}

bool TrafficLightController::greenOn() const
{
    return m_greenOn;
}

//! [1]
void TrafficLightController::setGreenOn(bool on)
{
    if (m_greenOn != on) {
        m_greenOn = on;
        emit greenOnChanged();
    }
}
//! [1]

bool TrafficLightController::paused() const
{
    return m_paused;
}

void TrafficLightController::setPaused(bool paused)
{
    if (m_paused != paused) {
        m_paused = paused;
        emit pausedChanged();
    }
}

//! [2]
void TrafficLightController::pause()
{
    emit pauseRequested();
}
//! [2]

//! [3]
void TrafficLightController::setupStateMachine()
{
    /**
     * The state machine consists of two top-level states
     *   - runningState
     *   - pausedState
     *
     * The runningState consists of 4 child states that represent the 4 phase
     * of a traffic light cycle:
     *   - stopState:   The red light is on, cars have to wait
     *   - waitState:   The red and yellow lights are on, cars can prepare to drive
     *   - driveState:  The green light is on, cars can drive
     *   - slowState:   The yellow light is on, cars have to slow down and stop
     *
     * Additionally the runningState contains a QHistoryState as child state, which is
     * used when going back from pausedState to runningState to ensure that we continue
     * execution at the same state we have been before we switched to the pausedState.
     */

    // Create the state machine
    QStateMachine *machine = new QStateMachine(this);

    // Create the two top-level states
    QState *runningState = new QState(machine);
    QState *pausedState = new QState(machine);

    // Create the child states for the runningState
    QState *stopState = new QState(runningState);
    QState *waitState = new QState(runningState);
    QState *driveState = new QState(runningState);
    QState *slowState = new QState(runningState);

    QHistoryState *historyState = new QHistoryState(runningState);

    // Define which properties should be modified when entering a state:

    /**
     * Depending on whether we are in runningState or pausedState, the 'paused' property
     * of the TrafficLightControl object will be set to false/true.
     */
    runningState->assignProperty(this, "paused", false);
    pausedState->assignProperty(this, "paused", true);

    /**
     * For each phase state we define which light should be enabled and
     * how long the phase should last.
     */
    stopState->assignProperty(this, "redOn", true);
    stopState->assignProperty(this, "yellowOn", false);
    stopState->assignProperty(this, "greenOn", false);
    stopState->assignProperty(m_timer, "interval", 5000);

    waitState->assignProperty(this, "redOn", true);
    waitState->assignProperty(this, "yellowOn", true);
    waitState->assignProperty(this, "greenOn", false);
    waitState->assignProperty(m_timer, "interval", 2000);

    driveState->assignProperty(this, "redOn", false);
    driveState->assignProperty(this, "yellowOn", false);
    driveState->assignProperty(this, "greenOn", true);
    driveState->assignProperty(m_timer, "interval", 5000);

    slowState->assignProperty(this, "redOn", false);
    slowState->assignProperty(this, "yellowOn", true);
    slowState->assignProperty(this, "greenOn", false);
    slowState->assignProperty(m_timer, "interval", 2000);

    // Define the transitions between the single states:

    /**
     * We cycle through the phase states whenever the timer emits the timeout()
     * signal.
     */
    stopState->addTransition(m_timer, SIGNAL(timeout()), waitState);
    waitState->addTransition(m_timer, SIGNAL(timeout()), driveState);
    driveState->addTransition(m_timer, SIGNAL(timeout()), slowState);
    slowState->addTransition(m_timer, SIGNAL(timeout()), stopState);

    /**
     * If we are in runningState and the pauseRequested() signal is emitted (that happens
     * when the pause() slot has been invoked) then we go to the pausedState.
     * If we are now in the pausedState and the pauseRequested() signal is emitted again,
     * we go to the historyState which will forward us to the phase state we have been in
     * when we left the runningState.
     */
    runningState->addTransition(this, SIGNAL(pauseRequested()), pausedState);
    pausedState->addTransition(this, SIGNAL(pauseRequested()), historyState);

    // Define which child state should be the initial state
    runningState->setInitialState(stopState);
    machine->setInitialState(runningState);

    // Start the state machine
    machine->start();
}
//! [3]
