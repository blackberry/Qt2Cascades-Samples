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
#include <bb/cascades/ListView>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Page>

#include "app.hpp"
#include "regexp.hpp"

using namespace bb::cascades;

App::App()
    : m_regExp(new RegExp(this))
{
    // Loads the QML file
    QmlDocument *qml = QmlDocument::create("asset:///main.qml");
    if (!qml->hasErrors()) {
        // Make the RegExp and App object available to the UI as context properties
        qml->setContextProperty("_app", this);
        qml->setContextProperty("_regexp", m_regExp);

        Page *appPage = qml->createRootObject<Page>();
        if (appPage) {
            Application::instance()->setScene(appPage);
        }
    }
}

void App::setPatternSyntax(int selectedIndex)
{
    // Set the pattern syntax depending on the index that the user has selected in the DropDown in the UI
    switch (selectedIndex) {
    case 0:
        m_regExp->setPatternSyntax(QRegExp::RegExp);
        break;
    case 1:
        m_regExp->setPatternSyntax(QRegExp::RegExp2);
        break;
    case 2:
        m_regExp->setPatternSyntax(QRegExp::Wildcard);
        break;
    case 3:
        m_regExp->setPatternSyntax(QRegExp::FixedString);
        break;
    case 4:
        m_regExp->setPatternSyntax(QRegExp::W3CXmlSchema11);
        break;
    default:
        qDebug() << "Unknown regexp pattern syntax for selected index: " << selectedIndex;
        break;
    }
}
