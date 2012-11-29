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

#include "app.hpp"
#include "xbelwriter.hpp"
#include "xbelreader.hpp"

using namespace bb::cascades;

//! [0]
App::App()
{
    // Load the main QML file and make the App object available as context property
    QmlDocument *qml = QmlDocument::create("asset:///main.qml");
    if (!qml->hasErrors()) {
        qml->setContextProperty("_app", this);
        Page *appPage = qml->createRootObject<Page>();
        if (appPage) {
            Application::instance()->setScene(appPage);

            // Retrieve the tree container control from the QML file
            m_treeContainer = appPage->findChild<Container*>("treeContainer");
        }
    }
}
//! [0]

//! [1]
void App::load(const QString &fileName)
{
    // Do sanity check
    if (!m_treeContainer)
        return;

    // Update the status property
    m_status = tr("Loading...");
    emit statusChanged();

    // Clean all previous generated bookmark controls from the tree container
    m_treeContainer->removeAll();

    // Create the XBEL reader and pass the tree container it will work on
    XbelReader reader(m_treeContainer);

    // Open the XBEL file which the user has selected
    QFile file("app/native/assets/" + fileName);
    if (!file.open(QIODevice::ReadOnly))
        qWarning("unable to open file");

    // Parse XBEL file and generate the bookmark controls
    const bool ok = reader.read(&file);

    // Update the status property again
    if (ok)
        m_status = tr("Loaded successfully");
    else
        m_status = reader.errorString();

    emit statusChanged();
}
//! [1]

//! [2]
void App::save()
{
    // Do sanity check
    if (!m_treeContainer)
        return;

    // Update the status property
    m_status = tr("Saving...");
    emit statusChanged();

    const QString fileName("tmp/streambookmarks.xbel");

    // Open the target file where the modified XBEL document will be written to
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);

    // Create the XBEL writer on the tree container and let it generate the XBEL document from the bookmark controls
    XbelWriter writer(m_treeContainer);
    const bool ok = writer.writeFile(&file);

    // Update the status property again
    if (ok)
        m_status = tr("Saved successfully");
    else
        m_status = tr("Error while saving");

    emit statusChanged();
}
//! [2]

QString App::status() const
{
    return m_status;
}
