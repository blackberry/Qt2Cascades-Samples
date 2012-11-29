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

#include "MoveCommand.hpp"

#include <bb/cascades/AbsoluteLayoutProperties>

using namespace bb::cascades;

//! [0]
MoveCommand::MoveCommand(Control *control, const QPoint &source, const QPoint &target, QUndoCommand *parent)
    : QUndoCommand(parent)
    , m_control(control)
    , m_sourcePoint(source)
    , m_targetPoint(target)
{
    // Extract the title property that has been set on the Stone control in the QML file
    const QString title = m_control->property("title").toString();

    /**
     * Set a meaningful text for this command. This text will be used by QUndoStack::undoText() and QUndoStack::redoText()
     * when this command is on the top of the stack.
     */
    setText(QString("Move %1").arg(title));
}
//! [0]

//! [1]
void MoveCommand::redo()
{
    /**
     * This method is called when the user triggers the redo of a command.
     * We simply retrieve the AbsoluteLayoutProperties object of our Stone control and
     * set it to the target position.
     */
    AbsoluteLayoutProperties *properties = qobject_cast<AbsoluteLayoutProperties*>(m_control->layoutProperties());
    if (properties) {
        properties->setPositionX(m_targetPoint.x());
        properties->setPositionY(m_targetPoint.y());
    }
}
//! [1]

//! [2]
void MoveCommand::undo()
{
    /**
     * This method is called when the user triggers the undo of a command.
     * We simply retrieve the AbsoluteLayoutProperties object of our Stone control and
     * set it back to its source position.
     */
    AbsoluteLayoutProperties *properties = qobject_cast<AbsoluteLayoutProperties*>(m_control->layoutProperties());
    if (properties) {
        properties->setPositionX(m_sourcePoint.x());
        properties->setPositionY(m_sourcePoint.y());
    }
}
//! [2]
