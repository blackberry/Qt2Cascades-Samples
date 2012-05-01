/****************************************************************************
 **
 ** Portions Copyright (C) 2012 Research In Motion Limited.
 ** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MESSAGEBOXCONTROLLER_HPP
#define MESSAGEBOXCONTROLLER_HPP

#include "DialogController.hpp"

#include <QtCore/QObject>

/**
 * The MessageBoxController encapsulates the logic of
 * a message box. It consists of a title, a text and two buttons.
 * When executed, it will block the execution flow
 * until the user has pressed one of the buttons.
 */
class MessageBoxController : public DialogController
{
    Q_OBJECT

    // Makes the title of the message box available to the UI
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)

    // Makes the text of the message box available to the UI
    Q_PROPERTY(QString text READ text NOTIFY textChanged)

    // Makes the text of the first button available to the UI
    Q_PROPERTY(QString button1Text READ button1Text NOTIFY button1TextChanged)

    // Makes the text of the second button available to the UI
    Q_PROPERTY(QString button2Text READ button2Text NOTIFY button2TextChanged)

public:
    explicit MessageBoxController(QObject *parent = 0);

    /**
     * Describes the two buttons of the dialog.
     */
    enum Result
    {
        Button1, Button2
    };

    // Shows the message box with the given title, text and the two buttons
    Result exec(const QString &title, const QString &text, const QString &button1Text, const QString &button2Text);

    // The accessor methods for the properties
    QString title() const;
    QString text() const;
    QString button1Text() const;
    QString button2Text() const;

    // This method is invoked when the user clicks the first button in the UI
    Q_INVOKABLE void button1Clicked();

    // This method is invoked when the user clicks the second button in the UI
    Q_INVOKABLE void button2Clicked();

Q_SIGNALS:
    // The change notification signals of the properties
    void titleChanged();
    void textChanged();
    void button1TextChanged();
    void button2TextChanged();

private:
    // The title of the message box
    QString m_title;

    // The text of the message box
    QString m_text;

    // The text of the first button of the message box
    QString m_button1Text;

    // The text of the second button of the message box
    QString m_button2Text;

    // The button that has been pressed to close the message box
    Result m_buttonResult;
};

#endif
