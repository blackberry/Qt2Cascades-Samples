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

#include "GameController.hpp"

#include "Board.hpp"
#include "Player.hpp"

#include <QScriptEngine>

//! [0]
/**
 * This function is called by the script engine if the user creates new Player objects from
 * within the script. (e.g. 'var p = new Player(board)')
 */
static QScriptValue constructNewPlayer(QScriptContext *context, QScriptEngine *engine)
{
    /**
     * Since the Player class expects a Board object as parameter to the constructor we
     * have to pass it in the script and extract it here again.
     */
    Board *board = qobject_cast<Board*>(context->argument(0).toQObject());
    if (!board) {
        // If no valid Board object was passed, we throw an error...
        return context->throwError("Missing Board parameter in ctor");
    }

    // ... otherwise we return a new Player object whose lifetime is managed by the script engine
    return engine->newQObject(new Player(board), QScriptEngine::ScriptOwnership);
}
//! [0]

GameController::GameController(QObject *parent)
    : QObject(parent)
    , m_boardContainer(0)
    , m_board(0)
    , m_player(0)
    , m_scriptEngine(0)
{
}

QString GameController::scriptError() const
{
    return m_scriptError;
}

void GameController::setBoard(bb::cascades::Container *board)
{
    /**
     * Now that we know on which Container we are supposed to work, create
     * our Board and Player objects that handle the business logic.
     */
    m_boardContainer = board;
    m_board = new Board(m_boardContainer, this);
    m_board->reset();

    m_player = new Player(m_board, this);
    m_player->reset();

    reset();
}

//! [1]
void GameController::reset()
{
    // Reset the script engine

    /**
     * We have to delete the script engine with deleteLater() here, because this method is called
     * by the script engine itself, so we should not call 'delete m_scriptEngine' while it is used.
     */
    m_scriptEngine->deleteLater();
    m_scriptEngine = new QScriptEngine(this);

    /**
     * Create a wrapper object for the GameController itself since we want to
     * access its reset() method from within the scripts.
     */
    QScriptValue scriptController = m_scriptEngine->newQObject(this);

    // Export it to the script engine environment
    m_scriptEngine->globalObject().setProperty("controller", scriptController);

    // Create a wrapper object for the Board object...
    QScriptValue scriptBoard = m_scriptEngine->newQObject(m_board);

    // ... and export it to the script engine environment
    m_scriptEngine->globalObject().setProperty("board", scriptBoard);

    // Create a wrapper object for the Player object...
    QScriptValue scriptPlayer = m_scriptEngine->newQObject(m_player);

    // ... and export it to the script engine environment as well
    m_scriptEngine->globalObject().setProperty("player", scriptPlayer);

    // Make the constructor function for new Player objects known to the script engine environment
    QScriptValue createPlayerFunc = m_scriptEngine->newFunction(constructNewPlayer);
    m_scriptEngine->globalObject().setProperty("Player", createPlayerFunc);
}
//! [1]

//! [2]
void GameController::run(const QString &script)
{
    // Run the script that is passed in from the UI
    const QScriptValue result = m_scriptEngine->evaluate(script);

    // Update the scriptError property
    if (result.isError()) {
        m_scriptError = QString::fromLatin1("%1: %2").arg(result.property("lineNumber").toInt32())
                                                     .arg(result.toString());
    } else {
        m_scriptError.clear();
    }

    emit scriptErrorChanged();

    /**
     * We call collectGarbage() explicitly here to ensure that Player objects are deleted immediately
     * and their tiles will disappear from the UI.
     */
    m_scriptEngine->collectGarbage();
}
//! [2]
