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

#ifndef BOARD_HPP
#define BOARD_HPP

#include <QObject>
#include <bb/cascades/Container>

/**
 * The Board class contains all the business logic for handling the maze board.
 * It generates a distribution of blocks on the maze (either static or randomly as configurable via
 * the staticBlockDistribution property) and it also generates the tiles (ImageViews) that are displayed
 * in the UI.
 */
//! [0]
class Board : public QObject
{
    Q_OBJECT

    // The property that defines whether the block distribution is done statically or randomly
    Q_PROPERTY(bool staticBlockDistribution READ staticBlockDistribution WRITE setStaticBlockDistribution NOTIFY staticBlockDistributionChanged)

public:
    // We take the Container that represents the maze board as parameter
    Board(bb::cascades::Container *boardContainer, QObject *parent = 0);

    /**
     * This method returns whether a player can move to the given position.
     * The position is given in logical coordinates (0-9).
     */
    bool canMoveTo(int x, int y) const;

    // The accessor methods for the block distribution property
    bool staticBlockDistribution() const;
    void setStaticBlockDistribution(bool value);

    bb::cascades::Container *board() const;

public Q_SLOTS:
    /**
     * This method will reset the maze board and regenerate all blocks.
     * If the staticBlockDistribution is 'false', the blocks will be placed randomly.
     */
    void reset();

Q_SIGNALS:
    // The change notification signal for the block distribution property
    void staticBlockDistributionChanged();

private:
    // The Container object that represents the maze board in the UI
    bb::cascades::Container *m_board;

    // The list of block tiles that we created
    QVector<bb::cascades::Control*> m_blocks;

    // The map where we store the locations of the blocks inside the maze
    QVector<QVector<bool> > m_blockMap;

    // The block distribution property
    bool m_staticBlockDistribution;
};
//! [0]

#endif
