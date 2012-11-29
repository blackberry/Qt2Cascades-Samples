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

#include "GrueDetector.hpp"

#include <QtCore/QDebug>
#include <QtCore/QTimer>

//! [0]
GrueDetector::GrueDetector(QObject *parent)
    : QObject(parent)
    , m_chance(0)
    , m_darkTimer(new QTimer(this))
{
    // At first we have to connect to the sensor backend...
    if (!m_sensor.connectToBackend()) {
        qWarning() << "Cannot connect to light sensor backend!";
    }

    // ... and then add a filter that will process the reported data
    m_sensor.addFilter(this);

    // Start the gathering of data
    m_sensor.start();

    connect(m_darkTimer, SIGNAL(timeout()), this, SLOT(increaseChance()));
}
//! [0]

//! [1]
bool GrueDetector::filter(QLightReading *reading)
{
    if (reading->lux() < 10) { // Dark enough for meeting a Grue
        if (!m_darkTimer->isActive())
            m_darkTimer->start(1000);
    } else { // everything is fine...
        m_darkTimer->stop();
        m_chance = 0;
        emit chanceChanged();
    }

    // Do no further processing of the sensor data
    return false;
}
//! [1]

void GrueDetector::increaseChance()
{
    // Increase the chance put keep it at 100% max
    m_chance = qBound(0, m_chance + 10, 100);
    emit chanceChanged();
}

int GrueDetector::chance() const
{
    return m_chance;
}
