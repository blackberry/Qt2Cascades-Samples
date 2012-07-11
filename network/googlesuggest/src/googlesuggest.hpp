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

#ifndef GOOGLESUGGEST_HPP
#define GOOGLESUGGEST_HPP

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtNetwork/QNetworkAccessManager>

#include <bb/cascades/Application>
#include <bb/cascades/QListDataModel>

class QNetworkReply;

//! [1]
class GoogleSuggest: public QObject
{
    Q_OBJECT

    // This property is used to set the input text from the UI
    Q_PROPERTY(QString input READ input WRITE setInput NOTIFY inputChanged)

    // This property makes the model that contains the results available to the UI
    Q_PROPERTY(bb::cascades::DataModel* model READ model NOTIFY modelChanged)

public:
    GoogleSuggest();

    // The accessor methods for the properties
    QString input() const;
    void setInput(const QString &input);

    bb::cascades::DataModel* model() const;

Q_SIGNALS:
    // The change notification signals of the properties
    void inputChanged();
    void modelChanged();

    // This signal is emitted to trigger the ListView to clear its selection
    void clearSelection();

private Q_SLOTS:
    // This method starts the actual query to the Google web service
    void autoSuggest();

    // This method is called when the query provides new data
    void handleNetworkData(QNetworkReply *reply);

    // This method adds the parsed choices/hits information to the data model
    void showCompletions(const QStringList &choices, const QStringList &hits);

private:
    // The data model that contains the choices/hits information
    mutable bb::cascades::QMapListDataModel m_model;

    // The network manager that handles the communication with the web service
    QNetworkAccessManager m_networkManager;

    // The time object to delay the start of the query
    QTimer m_timer;

    // The data the user has typed in
    QString m_input;
};
//! [1]

#endif
