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

#include "transitions.hpp"
#include "factorial.hpp"

#include <QtCore/QStateMachine>

/**
 * This transition inherits from QSignalTransition and passes 'fact' and 'SIGNAL(xChanged(int))'
 * to the constructor of the base class. That means whenever 'fact' emits the 'xChanged(int)' signal,
 * the state machine will try to invoke this transition.
 */
FactorialLoopTransition::FactorialLoopTransition(Factorial *fact)
    : QSignalTransition(fact, SIGNAL(xChanged(int)))
    , m_fact(fact)
{
}

bool FactorialLoopTransition::eventTest(QEvent *e)
{
    // Check if the base class accepts this transition
    if (!QSignalTransition::eventTest(e))
        return false;

    /**
     * We have to evaluate the parameter that is passed with the 'xChanged(int)' signal,
     * so we cast the passed QEvent object to the special QStateMachine::SignalEvent.
     * The signal events provides access to the passed parameters via its arguments() method.
     */
    QStateMachine::SignalEvent *se = static_cast<QStateMachine::SignalEvent*>(e);

    // Allow the transition (that means return 'true') as long as x > 1
    return se->arguments().at(0).toInt() > 1;
}

void FactorialLoopTransition::onTransition(QEvent *e)
{
    // This method is called when the transition is executed

    // Retrieve the 'x' value from the signal event and the 'fac' value from the Factorial object
    QStateMachine::SignalEvent *se = static_cast<QStateMachine::SignalEvent*>(e);
    const int x = se->arguments().at(0).toInt();
    const int fac = m_fact->property("fac").toInt();

    // Update the 'fac' property of the Factorial object
    m_fact->setProperty("fac", x * fac);

    /**
     * Decrement the 'x' property of the Factorial object.
     *
     * Note: This will cause the Factorial object to emit the 'xChanged(int)' signal and
     *       that will trigger the state machine to invoke this transition again -> it loops.
     */
    m_fact->setProperty("x", x - 1);
}

/**
 * This transition inherits from QSignalTransition and passes 'fact' and 'SIGNAL(xChanged(int))'
 * to the constructor of the base class. That means whenever 'fact' emits the 'xChanged(int)' signal,
 * the state machine will try to invoke this transition.
 */
FactorialDoneTransition::FactorialDoneTransition(Factorial *fact)
    : QSignalTransition(fact, SIGNAL(xChanged(int)))
    , m_fact(fact)
{
}

bool FactorialDoneTransition::eventTest(QEvent *e)
{
    // Check if the base class accepts this transition
    if (!QSignalTransition::eventTest(e))
        return false;

    QStateMachine::SignalEvent *se = static_cast<QStateMachine::SignalEvent*>(e);

    // Allow the transition (that means return 'true') only if x <= 1
    return se->arguments().at(0).toInt() <= 1;
}
