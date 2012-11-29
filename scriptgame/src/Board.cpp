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

#include "Board.hpp"

#include <bb/cascades/ImageView>
#include <bb/cascades/Color>

using namespace bb::cascades;

const static int s_boardDimension = 9;
const static int s_tileSize = 50;

//! [0]
Board::Board(bb::cascades::Container *boardContainer, QObject *parent)
    : QObject(parent)
    , m_board(boardContainer)
    , m_staticBlockDistribution(true)
{
    // Initialize the random number generator so that we can use it for dynamic block distribution
    qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());

    // Initialize the block map with 'false' for all cells -> no block available
    for (int x = 0; x < s_boardDimension; ++x) {
        m_blockMap << QVector<bool>();
        for (int y = 0; y < s_boardDimension; ++y) {
            m_blockMap[x] << false;
        }
    }
}
//! [0]

bool Board::staticBlockDistribution() const
{
    return m_staticBlockDistribution;
}

void Board::setStaticBlockDistribution(bool value)
{
    if (m_staticBlockDistribution != value) {
        m_staticBlockDistribution = value;
        emit staticBlockDistributionChanged();
    }
}

bb::cascades::Container *Board::board() const
{
    return m_board;
}

//! [1]
bool Board::canMoveTo(int x, int y) const
{
    // We can't move beyond the borders of the board
    if (x < 0 || x >= s_boardDimension || y < 0 || y >= s_boardDimension)
        return false;

    // We can't move to a coordinate where a block is located
    return (m_blockMap[x][y] == false);
}
//! [1]

//! [2]
void Board::reset()
{
    // Remove all block controls from the board...
    foreach(Control *block, m_blocks) {
        m_board->remove(block);
    }

    // ... and delete them
    qDeleteAll(m_blocks);
    m_blocks.clear();

    // Clear our internal block map
    for (int x = 0; x < s_boardDimension; ++x) {
        for (int y = 0; y < s_boardDimension; ++y) {
            m_blockMap[x][y] = false;
        }
    }

    QVector<QPoint> blockCoordinates;

    if (m_staticBlockDistribution) {
        // For static block distribution we use hard-coded coordinates
        blockCoordinates << QPoint(2, 8) << QPoint(5, 5) << QPoint(5, 0) << QPoint(7, 0)
                        << QPoint(7, 7) << QPoint(2, 7) << QPoint(1, 4) << QPoint(0, 5)
                        << QPoint(1, 8) << QPoint(4, 0) << QPoint(6, 3) << QPoint(6, 4)
                        << QPoint(2, 0) << QPoint(4, 5);
    } else {
        // For dynamic block distribution we use random coordinates
        for (int i = 0; i < 20; ++i) {
            const QPoint newPoint(qrand() % s_boardDimension, qrand() % s_boardDimension);

            if (blockCoordinates.contains(newPoint)) // contains a block already
                continue;

            if (newPoint == QPoint(0, 0)) // that's the starting place for the player
                continue;

            blockCoordinates << newPoint;
        }
    }

    // Generate the new blocks
    foreach (const QPoint position, blockCoordinates) {
        // Mark as occupied in blockMap
        m_blockMap[position.x()][position.y()] = true;

        // Create block tile
        ImageView *block = new ImageView();
        block->setPreferredWidth(50);
        block->setPreferredHeight(50);
        block->setImage(Image(QUrl("asset:///images/block.png")));
        block->setTranslationX(position.x() * s_tileSize);
        block->setTranslationY(position.y() * s_tileSize);

        // Add the block tile to the board container...
        m_board->add(block);

        // ... and store the object in our internal list, so that we can clean it up later on
        m_blocks << block;
    }
}
//! [2]
