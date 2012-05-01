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

#include <bb/cascades/CheckBox>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/TextArea>
#include <bb/cascades/TextField>

using namespace ::bb::cascades;

/**
 * A helper structure that represents a mapping of a control to a section.
 */
struct Mapping
{
    QPointer<Control> control;
    QString section;
    QByteArray propertyName;
};

/**
 * The private class that contains all private methods and member variables.
 * We use that concept to be able to change private API without influencing the
 * public API (see http://en.wikipedia.org/wiki/Opaque_pointer)
 */
class DataControlMapperPrivate
{
public:
    DataControlMapperPrivate()
        : m_model(0), m_currentIndex(-1)
    {
    }

    // A helper method to update the members of the given mapping with the given values
    void updateMapping(Mapping &mapping, Control *control, const QString &section, const QByteArray &propertyName);

    // This method does the actual data mapping from the model to the controls
    void update();

    // The data model we work on
    QPointer<bb::cascades::DataModel> m_model;

    // The list of mappings that have been set up
    QVector<Mapping> m_mappings;

    // The current index of the DataControlMapper
    int m_currentIndex;
};

void DataControlMapperPrivate::updateMapping(Mapping &mapping, Control *control, const QString &section, const QByteArray &propertyName)
{
    mapping.control = control;
    mapping.section = section;

    // If no property name is given we use the 'text' property as fallback
    mapping.propertyName = (propertyName.isEmpty() ? "text" : propertyName);
}

void DataControlMapperPrivate::update()
{
    // First some sanity checks
    if (!m_model)
        return;

    if (m_mappings.isEmpty())
        return;

    if (m_currentIndex == -1)
        return;

    // Extract the data for the current index (row) from the model
    const QVariant data = m_model->data(QVariantList() << QVariant(m_currentIndex));

    // We assume that the model returns one QVariantMap object per model entry (that's the case for most models)
    const QVariantMap map = data.toMap();
    if (map.isEmpty()) {
        qWarning("received incompatible record from model");
        return;
    }

    // Iterate over all available mappings
    foreach (const Mapping &mapping, m_mappings) {
        // Extract the actual value for the section identifier from the model data
        const QVariant value = map.value(mapping.section);
        if (mapping.control) {
            // Update the control's content by setting the property to the value
            mapping.control->setProperty(mapping.propertyName, value);
        }
    }
}

DataControlMapper::DataControlMapper(QObject *parent)
        : QObject(parent), d(new DataControlMapperPrivate())
{
}

DataControlMapper::~DataControlMapper()
{
    delete d;
}

void DataControlMapper::addMapping(bb::cascades::Control *control, const QString &section)
{
    // If no property name is provided, we use the well-known ones, depending on the control type
    QByteArray propertyName;
    if (qobject_cast<CheckBox*>(control))
        propertyName = "checked";
    else if (qobject_cast<ImageView*>(control))
        propertyName = "image";
    else if (qobject_cast<Label*>(control))
        propertyName = "text";
    else if (qobject_cast<TextArea*>(control))
        propertyName = "text";
    else if (qobject_cast<TextField*>(control))
        propertyName = "text";

    addMapping(control, section, propertyName);
}

void DataControlMapper::addMapping(bb::cascades::Control *control, const QString &section, const QByteArray &propertyName)
{
    // Check whether we have configured a mapping for this control already ...
    for (int i = 0; i < d->m_mappings.count(); ++i) {
        Mapping &mapping = d->m_mappings[i];
        if (mapping.control == control) {
            // ... in this case just update this mapping with the new section/propertyName values ...
            d->updateMapping(mapping, control, section, propertyName);

            // ... and trigger an update of the control content.
            d->update();
            return;
        }
    }

    // Otherwise create a new mapping ...
    Mapping mapping;
    d->updateMapping(mapping, control, section, propertyName);
    d->m_mappings.append(mapping);

    // ... and trigger an update of the control content.
    d->update();
}

void DataControlMapper::clearMapping()
{
    // Just clear our internal list of mappings
    d->m_mappings.clear();
}

int DataControlMapper::currentIndex() const
{
    return d->m_currentIndex;
}

int DataControlMapper::count() const
{
    if (!d->m_model)
        return 0;

    // Return the number of top-level rows of the model
    return d->m_model->childCount(QVariantList());
}

QByteArray DataControlMapper::mappedPropertyName(bb::cascades::Control *control) const
{
    foreach(const Mapping &mapping, d->m_mappings) {
        if (mapping.control == control)
        return mapping.propertyName;
    }

    return QByteArray();
}

QString DataControlMapper::mappedSection(bb::cascades::Control *control) const
{
    foreach(const Mapping &mapping, d->m_mappings) {
        if (mapping.control == control)
        return mapping.section;
    }

    return QString();
}

bb::cascades::Control* DataControlMapper::mappedControlAt(const QString &section) const
{
    foreach(const Mapping &mapping, d->m_mappings) {
        if (mapping.section == section)
        return mapping.control;
    }

    return 0;
}

bb::cascades::DataModel* DataControlMapper::model() const
{
    return d->m_model;
}

void DataControlMapper::removeMapping(bb::cascades::Control *control)
{
    for (int i = 0; i < d->m_mappings.count(); ++i) {
        if (d->m_mappings[i].control == control) {
            d->m_mappings.remove(i);
            return;
        }
    }
}

void DataControlMapper::setModel(bb::cascades::DataModel *model)
{
    d->m_model = model;

    // Reset the current index whenever a new model is set
    d->m_currentIndex = 0;

    // Trigger the update of the control's content
    d->update();
    emit countChanged();
}

void DataControlMapper::revert()
{
    // Just trigger the update of the control's content from the model data
    d->update();
}

void DataControlMapper::setCurrentIndex(int index)
{
    // Sanity check
    if (!d->m_model)
        return;

    // Get the number of top-level rows of the model ...
    const int rowCount = d->m_model->childCount(QVariantList());

    // ... and ignore invalid indexes.
    if (index < 0 || index >= rowCount)
        return;

    d->m_currentIndex = index;
    d->update();
    emit currentIndexChanged(d->m_currentIndex);
}

void DataControlMapper::toFirst()
{
    setCurrentIndex(0);
}

void DataControlMapper::toLast()
{
    if (!d->m_model)
        return;

    // Get the number of top-level rows of the model
    const int rowCount = d->m_model->childCount(QVariantList());

    setCurrentIndex(rowCount - 1);
}

void DataControlMapper::toNext()
{
    setCurrentIndex(d->m_currentIndex + 1);
}

void DataControlMapper::toPrevious()
{
    setCurrentIndex(d->m_currentIndex - 1);
}
