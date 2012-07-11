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

//! [0]
// Container for the visual representation of a XBEL bookmark element
Container {
    property string tagName: "bookmark"
    property alias title: titleField.text
    property alias url: urlField.text
    topMargin: 10
    leftMargin: 20

    layout: StackLayout {
        leftPadding: 20
    }

    Container {
        layout: StackLayout {
            layoutDirection: LayoutDirection.LeftToRight
        }

        // A standard ImageView
        ImageView {
            imageSource: "asset:///images/bookmark.png"
            minWidth: 82
            minHeight: 62
            layoutProperties: StackLayoutProperties {
                verticalAlignment: VerticalAlignment.Center
            }
            scalingMethod: ScalingMethod.AspectFit
        }

        // A standard Label for the bookmark title element
        Label {
            id: titleField
            leftMargin: 10
            layoutProperties: StackLayoutProperties {
                verticalAlignment: VerticalAlignment.Center
            }

            textStyle {
                base: SystemDefaults.TextStyles.SmallText
                color: Color.White
            }

            onTouch: {
                if (event.isDown()) {
                    urlField.visible = !urlField.visible
                }
            }
        }
    }

    // A standard TextField for the bookmark url attribute value
    TextField {
        id: urlField
        leftMargin: 10
        visible: false
        layoutProperties: StackLayoutProperties {
            verticalAlignment: VerticalAlignment.Center
        }

        textStyle {
            base: SystemDefaults.TextStyles.SmallText
        }
    }
}
//! [0]
