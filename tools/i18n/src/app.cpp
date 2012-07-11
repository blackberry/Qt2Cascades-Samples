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

#include <bb/cascades/Application>
#include <bb/cascades/Control>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Page>

#include <QtCore/QStringList>

#include "app.hpp"

using namespace bb::cascades;

//! [0]
/**
 * These static string literals will be used later on.
 * We use QT_TRANSLATE_NOOP here to just mark them as 'must-be-translated'
 * so that lupdate can extract them from the source code.
 */
static const char * const listEntries[] = {
    QT_TRANSLATE_NOOP("MainView", "First"),
    QT_TRANSLATE_NOOP("MainView", "Second"),
    QT_TRANSLATE_NOOP("MainView", "Third"),
    0
};
//! [0]

//! [1]
App::App()
    : m_translator(0)
{
    setupLanguageModel();

    QmlDocument *qml = QmlDocument::create().load("main.qml");
    if (!qml->hasErrors()) {
        /**
         * Make the language model available as context properties,
         * it is used by the ListView in the UI.
         */
        qml->setContextProperty("_model", &m_model);

        /**
         * Make the App object available to the UI as well, so that its setCurrentLanguage()
         * method can be invoked from there.
         */
        qml->setContextProperty("_app", this);

        Page *appPage = qml->createRootNode<Page>();
        if (appPage) {
            Application::instance()->setScene(appPage);

            /**
             * Lookup the main view and text area controls in the QML tree and store them
             * in our member variables.
             * Note: We use QPointers here for m_mainView and m_textArea, so we can always
             *       check whether the objects have been deleted by the UI in the meantime.
             */
            m_mainView = appPage->findChild<Control*>("mainView");
            m_textArea = appPage->findChild<TextArea*>("textArea");

            // Pre-select the second entry - English
            setCurrentLanguage(QVariantList() << QVariant(1));
        }
    }
}
//! [1]

//! [2]
void App::setCurrentLanguage(const QVariantList &indexPath)
{
    // Retrieve the currently selected language
    const QString language = m_model.data(indexPath).toString();

    // First remove any previous installed translator ...
    if (m_translator) {
        qApp->removeTranslator(m_translator);
        delete m_translator;
        m_translator = 0;
    }

    // ... then create and install a new translator for the selected language ...
    m_translator = new QTranslator(this);
    m_translator->load(m_languageMap.value(language));

    qApp->installTranslator(m_translator);

    // ... and trigger the translation of the texts inside the UI.
    if (m_mainView) {
        QMetaObject::invokeMethod(m_mainView, "retranslate");
    }

    if (m_textArea) {
        QString text;
        for (int i = 0; listEntries[i]; ++i) {
            /**
             * The static string literals, that have been marked to be translated with
             * QT_TRANSLATE_NOOP above, are eventually translated here.
             * Note: The context identifier ('MainView' here) must match with the context identifier
             *       that is used in the QT_TRANSLATE_NOOP declaration.
             */
            text += qApp->translate("MainView", listEntries[i]) + "\n";
        }

        m_textArea->setText(text);
    }
}
//! [2]

//! [3]
void App::setupLanguageModel()
{
    // Retrieve a list of all available translation catalog files ...
    const QStringList qmFiles = findQmFiles();

    // ... fill the internal language name -> catalog file map ...
    foreach (const QString &qmFile, qmFiles) {
        m_languageMap.insert(languageName(qmFile), qmFile);
    }

    // ... and also fill the language model with the language names.
    foreach (const QString &language, m_languageMap.keys()) {
        m_model << language;
    }
}
//! [3]

QStringList App::findQmFiles() const
{
    /**
     * Setup a QDir object that lists the content of the 'translations' directory.
     * The translations directory does not exists in the file system here but in the Qt Resource
     * system (that means the files are embedded inside the executable), therefor the
     * directory name is prefixed with ':/'.
     */
    const QDir dir(":/translations");

    // Retrieve all files ending with '.qm' sorted by name
    QStringList fileNames = dir.entryList(QStringList("*.qm"), QDir::Files, QDir::Name);

    // Iterate over the list and expand the file names to file paths
    QMutableStringListIterator it(fileNames);
    while (it.hasNext()) {
        it.next();
        it.setValue(dir.filePath(it.value()));
    }

    return fileNames;
}

QString App::languageName(const QString &qmFile) const
{
    /**
     * Create a temporary translator object and return the translated version of
     * the source string 'English'. In these example catalogs, this source string
     * has been translated to the language of the catalog (e.g. in i18n_de.qm 'English'
     * is translated to 'Deutsch' which means 'German').
     */
    QTranslator translator;
    translator.load(qmFile);

    return translator.translate("MainView", "English");
}
