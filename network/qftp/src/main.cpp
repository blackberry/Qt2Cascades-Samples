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

#include "FtpDownloader.hpp"
#include "FtpItemProvider.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/Page>
#include <bb/cascades/DataModel>
#include <bb/cascades/QmlDocument>

using namespace bb::cascades;

/**
 * This sample applications shows how to access remote data via the FTP protocol.
 * It implements a simple browser to view the content of an FTP server and allows
 * the user to download files to the local disk.
 */
int main(int argc, char **argv)
{
    Application app(argc, argv);

    // Creates the FtpDownloader object that contains the business logic
    FtpDownloader downloader;

    // Creates the FtpItemProvider for the ListView
    FtpItemProvider itemProvider;

    // Load the UI description from main.qml
    QmlDocument *qml = QmlDocument::create("asset:///main.qml");

    // Make all the business logic objects available to the UI as context properties
    qml->setContextProperty("_model", downloader.model());
    qml->setContextProperty("_itemProvider", &itemProvider);
    qml->setContextProperty("_downloader", &downloader);
    qml->setContextProperty("_messageBox", downloader.messageBoxController());
    qml->setContextProperty("_progressDialog", downloader.progressDialogController());

    // Create the application scene
    AbstractPane *appPage = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(appPage);

    // Quit the application if 'Qt.quit()' is called in the UI
    QObject::connect(qml->documentContext()->engine(), SIGNAL(quit()), &app, SLOT(quit()));

    return Application::exec();
}
