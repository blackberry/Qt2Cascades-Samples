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

#include "SensorExplorer.hpp"

using namespace bb::cascades;

/**
 * A helper function to convert the content of a QVariant to a QString
 * depending on the type of the content. We check for some well-known
 * types that are used by the sensor reading.
 */
static QString convertValue(const QString& type, const QVariant& value)
{
    if (type == "LightLevel") {
        switch (value.toInt()) {
            case 1: return "Dark";
            case 2: return "Twilight";
            case 3: return "Light";
            case 4: return "Bright";
            case 5: return "Sunny";
            default: return "Undefined";
        }
    } else if (type == "Orientation") {
        switch (value.toInt()) {
            case 1: return "TopUp";
            case 2: return "TopDown";
            case 3: return "LeftUp";
            case 4: return "RightUp";
            case 5: return "FaceUp";
            case 6: return "FaceDown";
            default: return "Undefined";
        }
    } else if (type == "qrangelist") {
        const qrangelist rangeList = value.value<qrangelist>();

        QStringList ranges;
        foreach (const qrange &range, rangeList) {
            if (range.first == range.second)
                ranges << QString("%1 Hz").arg(range.first);
            else
                ranges << QString("%1-%2 Hz").arg(range.first).arg(range.second);
        }

        if (ranges.count() > 0)
            return ranges.join(", ");

        return "-";
    } else if (type == "qoutputrangelist") {
        const qoutputrangelist rangeList = value.value<qoutputrangelist>();

        QStringList ranges;
        foreach (const qoutputrange &range, rangeList) {
            ranges << QString("(%1, %2) += %3").arg(range.minimum).arg(range.maximum).arg(range.accuracy);
        }

        if (ranges.count() > 0)
            return ranges.join(", ");

        return "-";
    } else if (type == "qtimestamp") {
        return QString("%1").arg(value.value<qtimestamp>());
    }

    return value.toString();
}

/**
 * A helper function to extract the type name from the meta data description
 * without any namespace.
 */
static QString typeName(const QMetaObject *metaObject, int propertyIndex)
{
    const QString typeName = QLatin1String(metaObject->property(propertyIndex).typeName());
    const int crap = typeName.lastIndexOf("::");
    if (crap != -1)
        return typeName.mid(crap + 2);

    return typeName;
}

/**
 * A helper function to hide certain properties in the UI.
 */
static bool ignoreProperty(const QString &propertyName)
{
    if (propertyName == "sensorid" ||
        propertyName == "reading" ||
        propertyName == "connected" ||
        propertyName == "running" ||
        propertyName == "supportsPolling")
        return true;

    return false;
}


//! [0]
SensorExplorer::SensorExplorer(QObject *parent)
    : QObject(parent)
    , m_sensorsModel(new GroupDataModel(this))
    , m_sensorPropertiesModel(new GroupDataModel(this))
    , m_readingPropertiesModel(new GroupDataModel(QStringList() << QLatin1String("position"), this))
    , m_sensor(0)
    , m_lastReading(0)
{
    // Don't use grouping in the models
    m_sensorsModel->setGrouping(ItemGrouping::None);
    m_sensorPropertiesModel->setGrouping(ItemGrouping::None);
    m_readingPropertiesModel->setGrouping(ItemGrouping::None);

    // Trigger the initial load of the available sensors
    loadSensors();
}
//! [0]

//! [1]
bool SensorExplorer::filter(QSensorReading *reading)
{
    // Limit update interval to 1 second
    if ((reading->timestamp() - m_lastReading) < 1000000)
        return false;

    m_lastReading = reading->timestamp();

    // Retrieve the meta data for the QSensorReading object ...
    const QMetaObject *metaObject = reading->metaObject();
    const int firstProperty = QSensorReading::staticMetaObject.propertyOffset();

    // ... and iterate over all its properties
    for (int i = firstProperty; i < metaObject->propertyCount(); ++i) {
        const int position = i - firstProperty;

        // Retrieve the type, name and value of the current property
        const QString type = typeName(metaObject, i);
        const QString name = QString::fromLatin1(metaObject->property(i).name());
        const QVariant value = metaObject->property(i).read(reading);

        // Fill the values in a new entry for the model
        QVariantMap readingEntry;
        readingEntry["position"] = position;
        readingEntry["type"] = type;
        readingEntry["name"] = name;
        readingEntry["value"] = convertValue(type, value);

        // Find the entry in the model that represents the current property (based on the position) ...
        const QVariantList indexPath = m_readingPropertiesModel->find(QVariantList() << position);

        // ... and update the entry with the new data
        m_readingPropertiesModel->updateItem(indexPath, readingEntry);
    }

    return false;
}
//! [1]

DataModel* SensorExplorer::sensorsModel() const
{
    return const_cast<GroupDataModel*>(m_sensorsModel);
}

DataModel* SensorExplorer::sensorPropertiesModel() const
{
    return const_cast<GroupDataModel*>(m_sensorPropertiesModel);
}

DataModel* SensorExplorer::readingPropertiesModel() const
{
    return const_cast<GroupDataModel*>(m_readingPropertiesModel);
}

bool SensorExplorer::sensorActive() const
{
    if (!m_sensor)
        return false;

    return m_sensor->isActive();
}

void SensorExplorer::setSensorActive(bool active)
{
    if (!m_sensor)
        return;

    if (m_sensor->isActive() != active) {
        m_sensor->setActive(active);
        emit sensorActiveChanged();
    }
}

//! [2]
void SensorExplorer::setCurrentSensor(const QVariantList &indexPath)
{
    // Sanity check
    if (indexPath.isEmpty())
        return;

    // Clear out anything that's in there now
    if (m_sensor) {
        delete m_sensor;
        m_sensor = 0;
    }

    m_sensorPropertiesModel->clear();
    m_readingPropertiesModel->clear();

    // Retrieve the selected sensor description from the model
    const QVariantMap sensorEntry = m_sensorsModel->data(indexPath).toMap();

    const QByteArray type = sensorEntry["type"].toString().toLatin1();
    const QByteArray identifier = sensorEntry["identifier"].toString().toLatin1();

    // Connect to the sensor so we can probe it
    m_sensor = new QSensor(type, this);
    connect(m_sensor, SIGNAL(readingChanged()), this, SLOT(sensorChanged()));
    connect(m_sensor, SIGNAL(activeChanged()), this, SIGNAL(sensorActiveChanged()));
    m_sensor->setIdentifier(identifier);
    if (!m_sensor->connectToBackend()) {
        delete m_sensor;
        m_sensor = 0;
        qWarning() << "Can't connect to the sensor!";
        return;
    }

    // Trigger the load of the sensor and reading properties
    loadSensorProperties();
    loadReadingProperties();
}
//! [2]

//! [3]
void SensorExplorer::loadSensors()
{
    // Clear out anything that's in there now
    m_sensorsModel->clear();

    // Iterate over all available sensor types
    foreach (const QByteArray &type, QSensor::sensorTypes()) {

        // Retrieve all identifiers for the current sensor type
        foreach (const QByteArray &identifier, QSensor::sensorsForType(type)) {

            if (type == "QHolsterSensor")
                continue; // prevent crash

            // Don't put in sensors we can't connect to
            QSensor sensor(type);
            sensor.setIdentifier(identifier);
            if (!sensor.connectToBackend()) {
                qDebug() << "Couldn't connect to" << identifier;
                continue;
            }

            // Create a new model entry for the current sensor instance ...
            QVariantMap sensorEntry;
            sensorEntry["identifier"] = identifier;
            sensorEntry["type"] = QString::fromLatin1(type);

            // ... and add it to the model
            m_sensorsModel->insert(sensorEntry);
        }
    }
}
//! [3]

void SensorExplorer::sensorChanged()
{
    filter(m_sensor->reading());
}

//! [4]
void SensorExplorer::loadSensorProperties()
{
    // Sanity check
    if (!m_sensor)
        return;

    // Retrieve the meta data for the QSensor object ...
    const QMetaObject *metaObject = m_sensor->metaObject();
    const int firstProperty = QSensor::staticMetaObject.propertyOffset();

    // ... and iterate over all its properties
    for (int i = firstProperty; i < metaObject->propertyCount(); ++i) {
        const QString name = QString::fromLatin1(metaObject->property(i).name());

        if (ignoreProperty(name)) // hide certain properties
            continue;

        const QString type = typeName(metaObject, i);
        const QVariant value = metaObject->property(i).read(m_sensor);

        // Create a new model entry for the current property ...
        QVariantMap sensorPropertiesEntry;
        sensorPropertiesEntry["type"] = type;
        sensorPropertiesEntry["name"] = name;
        sensorPropertiesEntry["value"] = convertValue(type, value);

        // ... and add it to the model
        m_sensorPropertiesModel->insert(sensorPropertiesEntry);
    }
}
//! [4]

//! [5]
void SensorExplorer::loadReadingProperties()
{
    const QSensorReading *reading = m_sensor->reading();

    // Retrieve the meta data for the QSensorReading object ...
    const QMetaObject *metaObject = reading->metaObject();
    const int firstProperty = QSensorReading::staticMetaObject.propertyOffset();

    // ... and iterate over all its properties
    for (int i = firstProperty; i < metaObject->propertyCount(); ++i) {
        const int position = i - firstProperty;

        const QString type = typeName(metaObject, i);
        const QString name = QString::fromLatin1(metaObject->property(i).name());
        const QVariant value = metaObject->property(i).read(reading);

        // Create a new model entry for the current property ...
        QVariantMap readingEntry;
        readingEntry["position"] = position;
        readingEntry["type"] = type;
        readingEntry["name"] = name;
        readingEntry["value"] = convertValue(type, value);

        // ... and add it to the model
        m_readingPropertiesModel->insert(readingEntry);
    }
}
//! [5]
