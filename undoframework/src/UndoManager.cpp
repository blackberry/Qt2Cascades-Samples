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

#include "UndoManager.hpp"

#include "MoveCommand.hpp"

#include <QUndoStack>

//! [0]
UndoManager::UndoManager(QObject *parent)
    : QObject(parent)
    , m_undoStack(new QUndoStack(this))
{
    // We connect the signals from the QUndoStack directly to our own signals ...
    connect(m_undoStack, SIGNAL(canRedoChanged(bool)), this, SIGNAL(canRedoChanged()));
    connect(m_undoStack, SIGNAL(canUndoChanged(bool)), this, SIGNAL(canUndoChanged()));
    connect(m_undoStack, SIGNAL(redoTextChanged(QString)), this, SIGNAL(redoTextChanged()));
    connect(m_undoStack, SIGNAL(undoTextChanged(QString)), this, SIGNAL(undoTextChanged()));
}
//! [0]

//! [1]
bool UndoManager::canRedo() const
{
    // ... and also just forward the getter methods to the QUndoStack
    return m_undoStack->canRedo();
}
//! [1]

bool UndoManager::canUndo() const
{
    return m_undoStack->canUndo();
}

QString UndoManager::redoText() const
{
    return m_undoStack->redoText();
}

QString UndoManager::undoText() const
{
    return m_undoStack->undoText();
}

//! [2]
void UndoManager::redo()
{
    m_undoStack->redo();
}
//! [2]

void UndoManager::undo()
{
    m_undoStack->undo();
}

//! [3]
void UndoManager::createMoveCommand(bb::cascades::Control *control, int sourceX, int sourceY, int targetX, int targetY)
{
    /**
     * We simply create a new instance of a MoveCommand, pass all needed data to its constructor and push
     * it on the QUndoStack.
     * The stack will take ownership of the command object, and will delete it when it's no longer used.
     */
    m_undoStack->push(new MoveCommand(control, QPoint(sourceX, sourceY), QPoint(targetX, targetY)));
}
//! [3]
