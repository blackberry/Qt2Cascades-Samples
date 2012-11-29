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

#ifndef GRUEDETECTOR_HPP
#define GRUEDETECTOR_HPP

#include <QtCore/QObject>
#include <QtSensors/QLightSensor>

QTM_USE_NAMESPACE

class QTimer;

/**
 * The GrueDetector class uses the QLightSensor class from the QtSensors
 * module to retrieve the current lux value from the light sensor of the device.
 */
//! [0]
class GrueDetector : public QObject, public QLightFilter
{
    Q_OBJECT

    // The property to access the chance value of being eaten by a grue
    Q_PROPERTY(int chance READ chance NOTIFY chanceChanged)

public:
    GrueDetector(QObject *parent = 0);

Q_SIGNALS:
    // The change notification signal of the chance property
    void chanceChanged();

protected:
    /**
     * This method is reimplemented from the QLightFilter interface and is
     * called by the QLightSensor whenever new values are available.
     */
    bool filter(QLightReading *reading);

private Q_SLOTS:
    // This method is called every second if we are in an dark environment
    void increaseChance();

private:
    // The accessor method for the chance property
    int chance() const;

    // The value of the chance property
    int m_chance;

    // The light sensor
    QLightSensor m_sensor;

    // A timer to calculate the chance
    QTimer *m_darkTimer;
};
//! [0]

#endif
