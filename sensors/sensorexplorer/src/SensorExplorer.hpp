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

#ifndef SENSOREXPLORER_HPP
#define SENSOREXPLORER_HPP

#include <QtCore/QObject>
#include <QtSensors/QSensor>

#include <bb/cascades/GroupDataModel>

QTM_USE_NAMESPACE

//! [0]
class SensorExplorer : public QObject, public QSensorFilter
{
    Q_OBJECT

    Q_PROPERTY(bb::cascades::DataModel* sensorsModel READ sensorsModel CONSTANT)
    Q_PROPERTY(bb::cascades::DataModel* sensorPropertiesModel READ sensorPropertiesModel CONSTANT)
    Q_PROPERTY(bb::cascades::DataModel* readingPropertiesModel READ readingPropertiesModel CONSTANT)
    Q_PROPERTY(bool sensorActive READ sensorActive WRITE setSensorActive NOTIFY sensorActiveChanged)

public:
    SensorExplorer(QObject *parent = 0);

    bool filter(QSensorReading *reading);

    bb::cascades::DataModel* sensorsModel() const;
    bb::cascades::DataModel* sensorPropertiesModel() const;
    bb::cascades::DataModel* readingPropertiesModel() const;

    bool sensorActive() const;
    void setSensorActive(bool active);

public Q_SLOTS:
    void setCurrentSensor(const QVariantList &indexPath);

Q_SIGNALS:
    void sensorActiveChanged();

private Q_SLOTS:
    void loadSensors();
    void sensorChanged();

private:
    void loadSensorProperties();
    void loadReadingProperties();

    bb::cascades::GroupDataModel *m_sensorsModel;
    bb::cascades::GroupDataModel *m_sensorPropertiesModel;
    bb::cascades::GroupDataModel *m_readingPropertiesModel;

    QSensor *m_sensor;
    qtimestamp m_lastReading;
};
//! [0]

#endif
