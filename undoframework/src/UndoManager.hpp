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

#ifndef UNDOMANAGER_HPP
#define UNDOMANAGER_HPP

#include <QObject>
#include <bb/cascades/Control>

class QUndoStack;

/**
 * The UndoManager wraps an instance of QUndoStack and makes its state available
 * via properties so that we can access them within the UI.
 */
class UndoManager : public QObject
{
    Q_OBJECT

    // These properties define whether there are commands on the stack that can be undone or re-done
    Q_PROPERTY(bool canRedo READ canRedo NOTIFY canRedoChanged)
    Q_PROPERTY(bool canUndo READ canUndo NOTIFY canUndoChanged)

    // These properties provide the text for the undo and redo button
    Q_PROPERTY(QString redoText READ redoText NOTIFY redoTextChanged)
    Q_PROPERTY(QString undoText READ undoText NOTIFY undoTextChanged)

public:
    UndoManager(QObject *parent = 0);

    // The accessor methods for our properties
    bool canRedo() const;
    bool canUndo() const;
    QString redoText() const;
    QString undoText() const;

    /**
     * This method is called by the UI whenever the user has moved a Stone.
     * As a result it will push a new MoveCommand on the internal QUndoStack.
     */
    Q_INVOKABLE void createMoveCommand(bb::cascades::Control *control, int sourceX, int sourceY, int targetX, int targetY);

public Q_SLOTS:
    /**
     * This method is called by the UI whenever the user presses the 'Redo' button
     */
    void redo();

    /**
     * This method is called by the UI whenever the user presses the 'Undo' button
     */
    void undo();

Q_SIGNALS:
    // The change notification signals for our properties
    void canRedoChanged();
    void canUndoChanged();
    void redoTextChanged();
    void undoTextChanged();

private:
    // The wrapped QUndoStack instance
    QUndoStack *m_undoStack;
};

#endif
