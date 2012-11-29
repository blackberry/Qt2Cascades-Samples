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

#ifndef PROGRESSDIALOGCONTROLLER_HPP
#define PROGRESSDIALOGCONTROLLER_HPP

#include "DialogController.hpp"

/**
 * The ProgressDialogController encapsulates the logic of
 * a progress dialog. It consists of a numerical progress value (in percent)
 * and a textual description.
 */
class ProgressDialogController : public DialogController
{
    Q_OBJECT

    // Make the progress information available to the UI
    Q_PROPERTY(float progress READ progress NOTIFY progressChanged)

    // Make the textual description available to the UI
    Q_PROPERTY(QString labelText READ labelText NOTIFY labelTextChanged)

public:
    explicit ProgressDialogController(QObject *parent = 0);

    // The accessor methods of the properties
    float progress() const;
    void setProgress(float progress);

    QString labelText() const;
    void setLabelText(const QString labelText);

    // Calling this method shows the progress dialog
    void show();

    // Calling this method hides the progress dialog
    void hide();

    // This method is invoked from the UI to cancel the task
    Q_INVOKABLE void cancel();

Q_SIGNALS:
    // The change notification signals of the properties
    void progressChanged();
    void labelTextChanged();

    // This signal is emitted when the user has canceled the task
    void canceled();

private:
    // The numeric progress value
    int m_progress;

    // The textual description
    QString m_labelText;
};

#endif
