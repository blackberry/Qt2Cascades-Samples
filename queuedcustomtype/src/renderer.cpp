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

#include "renderer.hpp"

#include <bb/cascades/AbsoluteLayoutProperties>

using namespace ::bb::cascades;

//! [Renderer constructor start]
Renderer::Renderer(QObject *parent)
    : QObject(parent), m_active(false)
{
    /**
     * We use only one RenderThread object in this example, so only one
     * image can be transformed at a time.
     */
    m_thread = new RenderThread();
//! [Renderer constructor start]

    // Connect the signals of the render thread against our own slots.
//! [set up widgets and connections]
    connect(m_thread, SIGNAL(finished()), this, SLOT(threadFinished()));
//! [set up widgets and connections]
//! [connecting signal with custom type]
    connect(m_thread, SIGNAL(sendBlock(Block)), this, SLOT(addBlock(Block)));
//! [connecting signal with custom type]
}

void Renderer::setCanvas(bb::cascades::Container *container)
{
    m_container = container;
}

void Renderer::loadImage(const QString &name)
{
    // Remove all blocks (from a previous transformation) from the canvas container
    m_container->removeAll();

    // Load the image from the file system and scale it down to 300x300 pixels
    QImage image;
    image.load(QString::fromLatin1("app/native/assets/images/%1").arg(name));
    image = image.scaled(300, 300);

    // Start the transformation of the image
    m_thread->processImage(image);

    // Adapt the 'active' property to the new state
    m_active = true;
    emit activeChanged();
}

void Renderer::stop()
{
    // Trigger the abortion of the worker thread
    m_thread->stopProcess();
}

//! [Adding blocks to the display]
void Renderer::addBlock(const Block &block)
{
    /**
     * We represent each block by a 'pixel' Container object with the same
     * geometry and color as the block.
     * The pixel containers are placed on the canvas container.
     */
    Container *pixel = new Container(m_container);
    pixel->setBackground(block.color());
    pixel->setPreferredWidth(block.rect().width());
    pixel->setPreferredHeight(block.rect().height());

    AbsoluteLayoutProperties *properties = new AbsoluteLayoutProperties;
    properties->setPositionX(block.rect().x());
    properties->setPositionY(block.rect().y());
    pixel->setLayoutProperties(properties);
}
//! [Adding blocks to the display]

void Renderer::threadFinished()
{
    // Reset the 'active' property whenever the worker thread has finished.
    m_active = false;
    emit activeChanged();
}

bool Renderer::active() const
{
    return m_active;
}
