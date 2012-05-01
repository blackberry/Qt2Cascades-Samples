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

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "renderthread.hpp"

#include <bb/cascades/Container>

//! [Renderer class definition]
/**
 * The Renderer class encapsulates business logic of this application.
 * It transforms an image by spawning off worker threads (RenderThread), retrieves
 * the result via a signal/slot connection and then 'paints' the result in
 * the UI by generating a couple of colored Containers inside a canvas Container.
 */
class Renderer : public QObject
{
    Q_OBJECT

    // A property that reports whether a worker thread is currently running.
    Q_PROPERTY(bool active READ active NOTIFY activeChanged)

public:
    Renderer(QObject *parent = 0);

    /**
     * This method is called to tell the Renderer on which Container the colored
     * blocks should be positioned.
     */
    void setCanvas(bb::cascades::Container *container);

    // This method is called whenever the user selects an image in the UI.
    Q_INVOKABLE void loadImage(const QString &name);

public Q_SLOTS:
    // This method is called whenever the user wants to abort the current transformation of an image
    void stop();

Q_SIGNALS:
    // The change notification signal for the active property
    void activeChanged();

private Q_SLOTS:
    // This method is called whenever the RenderThread has finished the calculation of one block.
    void addBlock(const Block &block);

    // This method is called whenever the RenderThread has finished the complete transformation of an image
    void threadFinished();

private:
    // The accessor method for the active property
    bool active() const;

    // The canvas container object
    bb::cascades::Container *m_container;

    // The render thread
    RenderThread *m_thread;

    // The active property
    bool m_active;
};
//! [Renderer class definition]

#endif
