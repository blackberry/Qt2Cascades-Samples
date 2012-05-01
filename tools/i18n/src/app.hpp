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

#ifndef APP_HPP
#define APP_HPP

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtCore/QMap>

#include <bb/cascades/Control>
#include <bb/cascades/Event>
#include <bb/cascades/QListDataModel>
#include <bb/cascades/TextArea>
#include <bb/cascades/UiObject>

class QStringList;

/**
 * This sample application shows how to use the internationalization infrastructure of Qt
 * to translate an application to multiple language.
 */
class App : public QObject
{
    Q_OBJECT

public:
    App();

    // This method is called when the user selects a different language in the UI
    Q_INVOKABLE void setCurrentLanguage(const QVariantList &indexPath);

private:
    // A helper method that creates the model for the language list view
    void setupLanguageModel();

    // A helper method that finds all available translation catalogs
    QStringList findQmFiles() const;

    // A helper method that returns the name of a language for a given catalog file
    QString languageName(const QString &qmFile) const;

    // The main view object in the UI
    QPointer<bb::cascades::Control> m_mainView;

    // The text area object in the UI
    QPointer<bb::cascades::TextArea> m_textArea;

    // The list model that contains the language names of all available catalogs
    bb::cascades::QListDataModel<QString> m_model;

    // The map that maps language names to catalog file names
    QMap<QString, QString> m_languageMap;

    // The translator object
    QTranslator *m_translator;
};

#endif
