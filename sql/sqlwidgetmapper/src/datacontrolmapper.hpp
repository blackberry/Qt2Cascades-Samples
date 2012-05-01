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

#ifndef DATACONTROLMAPPER_HPP
#define DATACONTROLMAPPER_HPP

#include <QtCore/QObject>

#include <bb/cascades/Control>
#include <bb/cascades/DataModel>

class DataControlMapperPrivate;

/**
 * The DataControlMapper class provides mapping between a row of a data model to controls.
 * DataControlMapper can be used to create data-aware controls by mapping them to rows of a data model.
 * Everytime the current index changes, each control is updated with data from the model.
 */
class DataControlMapper: public QObject
{
    Q_OBJECT

    /**
     * The current index of the DataControlMapper. If a SQL model on a SQL table is used, then the
     * index will be between 0 and the number of rows.
     */
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)

    // The number of rows the model provides
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    DataControlMapper(QObject *parent = 0);

    ~DataControlMapper();

    /**
     * This method creates a mapping between a control and a section identifier.
     * For a SQL model the section identifier is the column name.
     * The data will be set on well-known properties depending on the type of the control
     * (e.g. 'text' property for Label).
     */
    void addMapping(bb::cascades::Control *control, const QString &section);

    /**
     * This method creates a mapping between a control and a section identifier.
     * For a SQL model the section identifier is the column name.
     * The data will be set on the property with the given name.
     */
    void addMapping(bb::cascades::Control *control, const QString &section, const QByteArray &propertyName);

    /**
     * Removes all mappings that have been created before with addMapping().
     */
    void clearMapping();

    // Removes the mapping for the given control
    void removeMapping(bb::cascades::Control *control);

    // Accessor methods for the properties
    int currentIndex() const;
    int count() const;

    // Returns the property name that is used to update values for the given control
    QByteArray mappedPropertyName(bb::cascades::Control *control) const;

    // Returns the section identifier that is mapped to the given control
    QString mappedSection(bb::cascades::Control *control) const;

    // Returns the control that is mapped to the given section identifier
    bb::cascades::Control* mappedControlAt(const QString &section) const;

    // Returns the data model the DataControlMapper works on
    bb::cascades::DataModel* model() const;

    // Sets the data model the DataControlMapper will work on
    void setModel(bb::cascades::DataModel *model);

public Q_SLOTS:
    // This method resets the control's content with the data from the model
    void revert();

    // This method sets the current index to a given value
    void setCurrentIndex(int index);

    // This method sets the current index to the first row
    void toFirst();

    // This method sets the current index to the last row
    void toLast();

    // This method increments the current index by one row
    void toNext();

    // This method decrements the current index by one row
    void toPrevious();

Q_SIGNALS:
    // The change notification signals for the properties
    void currentIndexChanged(int index);
    void countChanged();

private:
    // The private class that hides private API
    DataControlMapperPrivate* const d;
};

#endif
