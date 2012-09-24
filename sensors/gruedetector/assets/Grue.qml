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

Container {
    id: root

    property int chance: 0

    layout: AbsoluteLayout {}

    preferredWidth: 768
    preferredHeight: 780

    ImageView {
        preferredWidth: 768

        layoutProperties: AbsoluteLayoutProperties {
            positionX: 0
            positionY: 290
        }

        imageSource: "asset:///images/insidemouth.png"
    }

    ImageView {
        preferredWidth: 768

        layoutProperties: AbsoluteLayoutProperties {
            positionX: 0
            positionY: root.chance <= 90 ? 410 : 630
        }

        imageSource: "asset:///images/downteeth.png"
    }

    ImageView {
        preferredWidth: 768

        layoutProperties: AbsoluteLayoutProperties {
            positionX: -10
            positionY: root.chance <= 90 ? -180 : -200
        }

        imageSource: "asset:///images/upteeth.png"
    }

    Container {
        background: Color.Black
        preferredWidth: 768
        preferredHeight: 400

        layoutProperties: AbsoluteLayoutProperties {
            positionX: 20
            positionY: 0
        }

        layout: AbsoluteLayout {}

        Eye {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 210
                positionY: 50
            }

            scaleX: 0.6
            scaleY: 0.6
        }

        Eye {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 350
                positionY: 30
            }

            scaleX: 0.25
            scaleY: 0.25
        }

        Eye {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 30
                positionY: 150
            }

            scaleX: 0.28
            scaleY: 0.28
        }

        Eye {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 190
                positionY: 210
            }

            scaleX: 0.21
            scaleY: 0.21
        }

        Eye {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 390
                positionY: 190
            }

            scaleX: 0.32
            scaleY: 0.32
        }

        Eye {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 490
                positionY: 90
            }

            scaleX: 0.1
            scaleY: 0.1
        }
    }
}
