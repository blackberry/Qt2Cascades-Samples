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

#ifndef GAMECONTROLLER_HPP
#define GAMECONTROLLER_HPP

#include <bb/cascades/Container>

#include <QObject>
#include <QScriptEngine>

class Board;
class Player;

/**
 * The GameController is the central class of this application.
 * It contains the classes that encapsulate the business logic (Board and Player)
 * and also the QScriptEngine, which is responsible for changing the properties of
 * Board and Player according to the JavaScript input.
 */
class GameController : public QObject
{
    Q_OBJECT

    // A custom property to make error messages from the script engine available to the UI
    Q_PROPERTY(QString scriptError READ scriptError NOTIFY scriptErrorChanged)

public:
    GameController(QObject *parent = 0);

    // The accessor method for the script error property
    QString scriptError() const;

    /**
     * With this method we tell the GameController on which Container object it should
     * run the game.
     */
    void setBoard(bb::cascades::Container *board);

    /**
     * Resets the script engine to an initial state. Since reset() is not a slot, we have to
     * mark it with Q_INVOKABLE to make it accessible from the script engine environment.
     */
    Q_INVOKABLE void reset();

public Q_SLOTS:
    // This method is called whenever the user clicks the 'Run Script' button in the UI
    void run(const QString &script);

Q_SIGNALS:
    // The change notification signal for the script error property
    void scriptErrorChanged();

private:
    // The Container object the game is run on
    bb::cascades::Container *m_boardContainer;

    // The Board object that contains the business logic for the maze board
    Board *m_board;

    // The Player object that contains the business logic for the main player
    Player *m_player;

    // The script error property
    QString m_scriptError;

    // The global script engine of this application
    QScriptEngine *m_scriptEngine;
};

#endif
