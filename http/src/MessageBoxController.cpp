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

#include "MessageBoxController.hpp"

MessageBoxController::MessageBoxController(QObject *parent)
    : DialogController(parent)
{
}

MessageBoxController::Result MessageBoxController::exec(const QString &title,
                                                        const QString &text, const QString &button1Text,
                                                        const QString &button2Text)
{
    // Store the passed values in local properties
    m_button1Text = button1Text;
    m_button2Text = button2Text;
    m_title = title;
    m_text = text;
    emit button1TextChanged();
    emit button2TextChanged();
    emit titleChanged();
    emit textChanged();

    // Start the nested event loop by calling the base class' exec() method
    DialogController::exec();

    // Return the button that has been clicked to close the message box
    return m_buttonResult;
}

QString MessageBoxController::title() const
{
    return m_title;
}

QString MessageBoxController::text() const
{
    return m_text;
}

QString MessageBoxController::button1Text() const
{
    return m_button1Text;
}

QString MessageBoxController::button2Text() const
{
    return m_button2Text;
}

void MessageBoxController::button1Clicked()
{
    // Set the button flag to the first button
    m_buttonResult = Button1;

    // Close the message box
    close();
}

void MessageBoxController::button2Clicked()
{
    // Set the button flag to the second button
    m_buttonResult = Button2;

    // Close the message box
    close();
}
