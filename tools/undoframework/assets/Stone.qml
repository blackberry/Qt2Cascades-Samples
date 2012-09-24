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

import bb.cascades 1.0

ImageView {
    // The custom property to define the color of the stone
    property string color : "white"

    // The custom property to define the name of the stone
    property string title

    // Local properties for position calculation
    property int startX
    property int startY
    property int offsetX
    property int offsetY

//! [0]
    // This signal is emitted after each move operation
    signal moved(int sourceX, int sourceY, int targetX, int targetY)
//! [0]

    imageSource: "asset:///images/" + color + "_stone.png"

    preferredWidth: 64
    preferredHeight: 64

    touchBehaviors: TouchBehavior {
        TouchReaction {
            eventType: TouchType.Down
            phase: PropagationPhase.AtTarget
            response: TouchResponse.StartTracking
        }
    }

    onTouch: {
        if (event.isDown()) {
            // Store original start position of the move
            startX = layoutProperties.positionX
            startY = layoutProperties.positionY

            offsetX = event.windowX - layoutProperties.positionX
            offsetY = event.windowY - layoutProperties.positionY
        } else if (event.isMove()) {
            // Move stone to current position
            layoutProperties.positionX = Math.min(Math.max(event.windowX - offsetX, 0), 436)
            layoutProperties.positionY = Math.min(Math.max(event.windowY - offsetY, 0), 436)
        } else if (event.isUp()) {
            // emit the moved() signal
            moved(startX, startY, layoutProperties.positionX, layoutProperties.positionY)
        }
    }
}
