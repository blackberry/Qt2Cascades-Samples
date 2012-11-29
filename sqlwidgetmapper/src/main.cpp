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

#include "datacontrolmapper.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/Control>
#include <bb/cascades/QListDataModel>
#include <bb/cascades/QmlDocument>
#include <bb/data/SqlDataAccess>

using namespace bb::cascades;
using namespace bb::data;

/**
 * This sample application shows how to read data from an QSLite database and displaying
 * them in the UI.
 * Additionally this example provides a class that maps a row of a SQL table on a
 * set of controls in the UI and allows the user to navigate throught the rows of the table
 * and update the contents of the controls automatically.
 */
Q_DECL_EXPORT int main(int argc, char **argv)
{
    Application app(argc, argv);

    const QString dbNameWithPath = "app/native/assets/sql/quotes.db";

    // Create a SqlDataAccess object for a SQLite database file in the filesystem ...
    SqlDataAccess sqlDataAccess(dbNameWithPath);

    // Load the result into a QVariantList ...
    const QVariantList sqlData = sqlDataAccess.execute("SELECT * FROM quotes").value<QVariantList>();

    // Use the result as data source for a QListDataModel
    QListDataModel<QVariantMap> model;

    foreach (const QVariant &entry, sqlData)
        model.append(entry.toMap());

//! [0]
    // Create a DataControlMapper and let it work on the DataSetModel
    DataControlMapper mapper;
    mapper.setModel(&model);

    // Load the UI description from main.qml
    QmlDocument *qml = QmlDocument::create("asset:///main.qml");

    // Make the DataControlMapper available to the UI as context property
    qml->setContextProperty("_dataMapper", &mapper);

    // Create the application scene
    AbstractPane *appPage = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(appPage);

    if (appPage) {
        // Lookup the controls that should contain the content of a row from the SQL data
        Control *firstNameField = appPage->findChild<Control*>("firstNameField");
        Control *lastNameField = appPage->findChild<Control*>("lastNameField");
        Control *quoteField = appPage->findChild<Control*>("quoteField");

        // Define the mapping between control and column name
        mapper.addMapping(firstNameField, "firstname");
        mapper.addMapping(lastNameField, "lastname");
        mapper.addMapping(quoteField, "quote");
    }
//! [0]

    return Application::exec();
}
