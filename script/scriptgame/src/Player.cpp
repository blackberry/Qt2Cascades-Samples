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

#include "Player.hpp"

#include "Board.hpp"

#include <bb/cascades/AbsoluteLayoutProperties>

using namespace bb::cascades;

const static int s_tileSize = 50;

//! [0]
Player::Player(Board *board, QObject *parent)
    : QObject(parent)
    , m_board(board)
    , m_playerTile(new ImageView)
    , m_currentDirection(Down)
    , m_currentPosition(0, 0)
{
    // Initialize the player tile and add it to the board container
    m_playerTile->setPreferredWidth(s_tileSize);
    m_playerTile->setPreferredHeight(s_tileSize);
    m_playerTile->setImage(Image(QUrl("asset:///images/player.png")));
    m_board->board()->add(m_playerTile);

    /**
     * Ensure that x/y position is really 0, otherwise using the translationX/translationY properties
     * does not work as expected.
     */
    AbsoluteLayoutProperties *properties = qobject_cast<AbsoluteLayoutProperties*>(m_playerTile->layoutProperties());
    if (properties) {
        properties->setPositionX(0);
        properties->setPositionY(0);
    }
}
//! [0]

Player::~Player()
{
    // Remove the tile from the UI when this player is deleted
    m_board->board()->remove(m_playerTile);
    delete m_playerTile;
}

void Player::reset()
{
    // Reset the current direction
    m_currentDirection = Down;
    m_playerTile->setRotationZ(0);

    // Reset the current position
    m_currentPosition = QPoint(0, 0);
    m_playerTile->setTranslationX(0);
    m_playerTile->setTranslationY(0);
}

//! [1]
void Player::turnLeft()
{
    // Just rotate the tile, in a later version we can use different images for each direction
    m_playerTile->setRotationZ(m_playerTile->rotationZ() - 90);

    // Update the direction depending on the current direction
    switch (m_currentDirection) {
        case Up:
            m_currentDirection = Left;
            break;
        case Right:
            m_currentDirection = Up;
            break;
        case Down:
            m_currentDirection = Right;
            break;
        case Left:
            m_currentDirection = Down;
            break;
    }
}
//! [1]

//! [2]
void Player::turnRight()
{
    // Just rotate the tile, in a later version we can use different images for each direction
    m_playerTile->setRotationZ(m_playerTile->rotationZ() + 90);

    // Update the direction depending on the current direction
    switch (m_currentDirection) {
        case Up:
            m_currentDirection = Right;
            break;
        case Right:
            m_currentDirection = Down;
            break;
        case Down:
            m_currentDirection = Left;
            break;
        case Left:
            m_currentDirection = Up;
            break;
    }
}
//! [2]

//! [3]
void Player::go()
{
    // Update the current position of the player depending on the current direction
    switch (m_currentDirection) {
        case Up:
            if (m_board->canMoveTo(m_currentPosition.x(), m_currentPosition.y() - 1)) {
                m_currentPosition.setY(m_currentPosition.y() - 1);
            }
            break;
        case Right:
            if (m_board->canMoveTo(m_currentPosition.x() + 1, m_currentPosition.y())) {
                m_currentPosition.setX(m_currentPosition.x() + 1);
            }
            break;
        case Down:
            if (m_board->canMoveTo(m_currentPosition.x(), m_currentPosition.y() + 1)) {
                m_currentPosition.setY(m_currentPosition.y() + 1);
            }
            break;
        case Left:
            if (m_board->canMoveTo(m_currentPosition.x() - 1, m_currentPosition.y())) {
                m_currentPosition.setX(m_currentPosition.x() - 1);
            }
            break;
    }

    // Update the position of the player tile on screen
    m_playerTile->setTranslationX(m_currentPosition.x() * s_tileSize);
    m_playerTile->setTranslationY(m_currentPosition.y() * s_tileSize);
}
//! [3]
