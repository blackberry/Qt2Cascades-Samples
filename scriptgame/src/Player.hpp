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

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QObject>
#include <bb/cascades/Container>
#include <bb/cascades/ImageView>

class Board;

/**
 * The Player class encapsulates the business logic of a player on the maze board.
 * It provides methods to change direction and position of the player.
 */
//! [0]
class Player : public QObject, public QScriptable
{
    Q_OBJECT

public:
    /**
     * The constructor takes the Board object, where the player should play on, as parameter
     */
    Player(Board *board, QObject *parent = 0);
    ~Player();

public Q_SLOTS:
    // This method can be called to reset the Player object to it's initial state
    void reset();

    // This method turns the player to the left by 90 degree
    void turnLeft();

    // This method turn the player to the right by 90 degree
    void turnRight();

    // This method moves the player by on step in its current direction
    void go();

private:
    /**
     * Describes the possible directions the player can move to
     */
    enum Direction {
        Up,
        Right,
        Down,
        Left
    };

    // The Board object the player is playing on
    Board *m_board;

    // The tile that represents the player in the UI
    bb::cascades::ImageView *m_playerTile;

    // The direction the player is currently moving
    Direction m_currentDirection;

    // The current position of the player on the board
    QPoint m_currentPosition;
};
//! [0]

#endif
