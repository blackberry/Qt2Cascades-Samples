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

#include <QtCore/QAbstractTransition>
#include <QtCore/QState>
#include <QtCore/QStateMachine>

using namespace ::bb::cascades;

/**
 * The TextBuffer is a helper class to make the output of the
 * 'pinger', 'pingTransition' and 'pongTransition' available to the UI.
 */
class TextBuffer : public QObject
{
    Q_OBJECT

    // A property that makes the output text available to the UI
    Q_PROPERTY(QString text READ text NOTIFY textChanged)

public:
    explicit TextBuffer(QObject *parent = 0)
        : QObject(parent)
    {
    }

    // The accessor method of the property
    QString text() const
    {
        return m_text;
    }

public Q_SLOTS:
    // This slot is called whenever new output should be appended to the text buffer
    void appendText(const QString &text)
    {
        m_text += text + "\n";

        // Limit the text size, too much text makes TextArea slow
        if (m_text.length() > 100) {
            m_text.clear();
        }

        emit textChanged();
    }

Q_SIGNALS:
    // The change notification signal of the property
    void textChanged();

private:
    // The output text
    QString m_text;
};

//! [0]
/**
 * The PingEvent class is a custom event that is sent by the pinger state
 * to the state machine to trigger a transition to the ponger state.
 */
class PingEvent : public QEvent
{
public:
    PingEvent()
        : QEvent(QEvent::Type(QEvent::User + 2))
    {
    }
};

/**
 * The PongEvent class is a custom event that is sent by the ponger state
 * to the state machine to trigger a transition to the pinger state.
 */
class PongEvent : public QEvent
{
public:
    PongEvent()
        : QEvent(QEvent::Type(QEvent::User + 3))
    {
    }
};
//! [0]

//! [1]
/**
 * The Pinger class is a custom state that sends a PingEvent to
 * the state machine whenever it is entered.
 */
class Pinger : public QState
{
    Q_OBJECT

public:
    Pinger(QState *parent)
        : QState(parent)
    {
    }

Q_SIGNALS:
    // This signal is emitted whenever the state is entered
    void notify(const QString &message);

protected:
    /**
     * This method is reimplemented from the QState class.
     * It is called whenever this state is entered.
     * Note: In this example it will start the ping/pong between the two transitions
     *       by sending the initial PingEvent.
     */
    virtual void onEntry(QEvent *)
    {
        // Post a PingEvent to the state machine ...
        machine()->postEvent(new PingEvent());

        // ... and add some text to the output on screen.
        emit notify("ping?");
    }
};
//! [1]

//! [3]
/**
 * The PongTransition is activated whenever the state machine receives a PongEvent.
 * In that case it will send a PingEvent to the state machine.
 */
class PongTransition : public QAbstractTransition
{
    Q_OBJECT

public:
    PongTransition()
    {
    }

Q_SIGNALS:
    // This signal is emitted whenever the transition is executed
    void notify(const QString &message);

protected:
    virtual bool eventTest(QEvent *e)
    {
        // Allow this transition only if the event is a PongEvent
        return (e->type() == QEvent::User + 3);
    }

    virtual void onTransition(QEvent *)
    {
        // Post a PingEvent to the state machine with a delay of 500ms  ...
        machine()->postDelayedEvent(new PingEvent(), 500);

        // ... and add some text to the output on screen.
        emit notify("ping?");
    }
};
//! [3]

//! [2]
/**
 * The PingTransition is activated whenever the state machine receives a PingEvent.
 * In that case it will send a PongEvent to the state machine.
 */
class PingTransition : public QAbstractTransition
{
    Q_OBJECT

public:
    PingTransition()
    {
    }

Q_SIGNALS:
    // This signal is emitted whenever the transition is executed
    void notify(const QString &message);

protected:
    virtual bool eventTest(QEvent *e)
    {
        // Allow this transition only if the event is a PingEvent
        return (e->type() == QEvent::User + 2);
    }

    virtual void onTransition(QEvent *)
    {
        // Post a PongEvent to the state machine with a delay of 500ms  ...
        machine()->postDelayedEvent(new PongEvent(), 500);

        // ... and add some text to the output on screen.
        emit notify("    pong!");
    }
};
//! [2]

//! [4]
/**
 * In this sample application a state machine is used to alternate between two actions (showing 'ping?' and 'pong!' in the UI).
 * To trigger the transitions between the two actions, custom events are used instead of signals.
 */
int main(int argc, char **argv)
{
    Application app(argc, argv);

    // Create the state machine
    QStateMachine machine;

    /**
     * Create the group state as parallel state.
     * That means its two child states 'pinger' and 'ponger'
     * will both be entered by the state machine at the same time.
     */
    QState *group = new QState(QState::ParallelStates);
    group->setObjectName("group");
//! [4]

//! [5]
    // Create the pinger state ...
    Pinger *pinger = new Pinger(group);
    pinger->setObjectName("pinger");

    // ... and add a PongTransition to it.
    PongTransition *pongTransition = new PongTransition();
    pinger->addTransition(pongTransition);

    // Create the ponger state ...
    QState *ponger = new QState(group);
    ponger->setObjectName("ponger");

    // ... and add a PingTransition to it.
    PingTransition *pingTransition = new PingTransition();
    ponger->addTransition(pingTransition);
//! [5]

//! [6]
    // Add the group state to the state machine ...
    machine.addState(group);

    // ... and mark it as initial state.
    machine.setInitialState(group);

    // Start the state machine
    machine.start();

    // Create the text buffer ...
    TextBuffer textBuffer;

    QmlDocument * const qml = QmlDocument::create().load("main.qml");
    if (!qml->hasErrors()) {
        // ... and make it available as context property to the UI.
        qml->setContextProperty("_textBuffer", &textBuffer);
        Page *appPage = qml->createRootNode<Page>();
        if (appPage) {
            Application::setScene(appPage);

            /**
             * Setup signal/slot connections to append the notification strings from
             * the 'pinger' and the 'pingTransition' and 'pongTransition' objects to
             * the text buffer.
             */
            QObject::connect(pinger, SIGNAL(notify(QString)), &textBuffer,
                    SLOT(appendText(QString)));
            QObject::connect(pingTransition, SIGNAL(notify(QString)), &textBuffer,
                    SLOT(appendText(QString)));
            QObject::connect(pongTransition, SIGNAL(notify(QString)), &textBuffer,
                    SLOT(appendText(QString)));
        }
    }
    return Application::exec();
}
//! [6]

#include "main.moc"
