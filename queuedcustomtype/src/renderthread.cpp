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

#include "renderthread.hpp"

RenderThread::RenderThread(QObject *parent)
    : QThread(parent)
{
    m_abort = false;
}

RenderThread::~RenderThread()
{
    /**
     * If the destructor of the RendererThread is called while the worker
     * thread is still running, we signal the worker thread that it should
     * abort execution and we block execution of this thread (the main thread),
     * until the worker thread has finished, by calling wait().
     */
    mutex.lock();
    m_abort = true;
    mutex.unlock();

    wait();
}

//![processing the image (start)]
void RenderThread::processImage(const QImage &image)
{
    if (image.isNull())
        return;

    // Store a copy of the image ...
    m_image = image;

    // ... reset the abort flag ...
    m_abort = false;

    // ... and start the execution of the worker thread.
    start();
}

void RenderThread::run()
{
    // Note: The content of this method is executed inside the worker thread!

    // Iterate over the image pixel-by-pixel and calculate the color of the blocks
    int size = qMax(m_image.width() / 20, m_image.height() / 20);
    for (int s = size; s > 10; --s) {
        for (int c = 0; c < 400; ++c) {
//![processing the image (start)]
            int x1 = qMax(0, (qrand() % m_image.width()) - s / 2);
            int x2 = qMin(x1 + s / 2 + 1, m_image.width());
            int y1 = qMax(0, (qrand() % m_image.height()) - s / 2);
            int y2 = qMin(y1 + s / 2 + 1, m_image.height());
            int n = 0;
            int red = 0;
            int green = 0;
            int blue = 0;
            for (int i = y1; i < y2; ++i) {
                for (int j = x1; j < x2; ++j) {
                    QRgb pixel = m_image.pixel(j, i);
                    red += qRed(pixel);
                    green += qGreen(pixel);
                    blue += qBlue(pixel);
                    n += 1;
                }
            }
//![processing the image (finish)]
            if (qAlpha(m_image.pixel(x1, y1)) != 0) { // ignore transparent pixels
                // Create a corresponding color object
                const bb::cascades::Color color = bb::cascades::Color::fromRGBA(red / n / 255.0, green / n / 255.0, blue / n / 255.0, 1.0);

                // Create a block of the current block size
                Block block(QRect(x1, y1, x2 - x1 + 1, y2 - y1 + 1), color);

                // Send the result by emitting the signal
                emit sendBlock(block);
            }

            /**
             * If the user requested the abort of the worker thread, we simply return from the run() method,
             * which will cause the termination of the worker thread.
             */
            if (m_abort)
                return;

            msleep(10);
        }
    }
}
//![processing the image (finish)]

void RenderThread::stopProcess()
{
    /**
     * If the user requests an abort of the worker thread, we set the m_abort flag
     * protected by the mutex.
     */
    mutex.lock();
    m_abort = true;
    mutex.unlock();
}
