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

#include "FileLoaderProxy.hpp"

//! [0]
// The key that is used for the shared memory segment
static const char *s_sharedKey = "fileloader_shm_key";

// The size of the shared memory segment
static const int s_memorySize = 10 * 1024; // 10kB
//! [0]

FileLoaderProxy::FileLoaderProxy(QObject *parent)
    : QObject(parent)
{
//! [1]
    // Define the key that identifies this shared memory segment
    m_sharedMemory.setKey(QString::fromLatin1(s_sharedKey));

    // Create the shared memory segment with the given size
    m_sharedMemory.create(s_memorySize);
//! [1]
}

//! [2]
QString FileLoaderProxy::fileContent() const
{
    // Before we access the content of the shared memory we should lock it
    m_sharedMemory.lock();

    // Read out all data
    QString content = QString::fromUtf8(static_cast<char*>(m_sharedMemory.data()));
    if (content.isEmpty())
        content = tr("No file loaded yet, please use sharedmemory_loader sample application to load one.");

    // Unlock the shared memory again
    m_sharedMemory.unlock();

    return content;
}
//! [2]

//! [3]
void FileLoaderProxy::loadFile()
{
    /**
     * We expect that the user has started the sharedmemory_loader example and loaded
     * a file into the shared memory already, so just trigger to read the data from the
     * shared memory.
     */
    emit fileContentChanged();
}
//! [3]
