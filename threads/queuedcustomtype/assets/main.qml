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

// Page shows a Layout of ImageView's, Label's and control Button
Page {
    Container {

        layout: DockLayout {}

        // The background image
        ImageView {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            imageSource: "asset:///images/background.png"
        }

        // Container for the title
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            // A standard Label
            Label {
                horizontalAlignment: HorizontalAlignment.Center
                topMargin: 20

                text: qsTr ("Touch one of the images below to start rendering")

                // Defines custom text Color and style
                textStyle {
                    base: SystemDefaults.TextStyles.SmallText
                    color: Color.Black
                }
            }

            // Groups the available ImageView's for rendering
            Container {
                horizontalAlignment: HorizontalAlignment.Center
                topMargin: 10

                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }

                // A standard ImageView
                ImageView {
                    preferredWidth: 128
                    preferredHeight: 128
                    imageSource: "asset:///images/box.png"

                    // Render this image on touch
                    onTouch: {
                        if (! _renderer.active) _renderer.loadImage ("box.png")
                    }
                }

                // A standard ImageView
                ImageView {
                    leftMargin: 30
                    preferredWidth: 128
                    preferredHeight: 128

                    imageSource: "asset:///images/sphere.png"

                    // Render this image on touch
                    onTouch: {
                        if (! _renderer.active) _renderer.loadImage ("sphere.png")
                    }
                }

                // A standard ImageView
                ImageView {
                    leftMargin: 30
                    preferredWidth: 128
                    preferredHeight: 128

                    imageSource: "asset:///images/cylinder.png"

                    // Render this image on touch
                    onTouch: {
                        if (! _renderer.active) _renderer.loadImage ("cylinder.png")
                    }
                }

                // A standard ImageView
                ImageView {
                    leftMargin: 30
                    preferredWidth: 128
                    preferredHeight: 128

                    imageSource: "asset:///images/pyramid.png"

                    // Render this image on touch
                    onTouch: {
                        if (! _renderer.active) _renderer.loadImage ("pyramid.png")
                    }
                }
            }

            // Container in which the image is rendered
            Container {
                objectName: "canvas"

                horizontalAlignment: HorizontalAlignment.Center
                preferredWidth: 300
                preferredHeight: 300

                layout: AbsoluteLayout {}
            }

            // Container for the app control Button
            Container {
                horizontalAlignment: HorizontalAlignment.Center
                bottomPadding: 5

                preferredWidth: 200

                // A standard Button
                Button {
                    text: qsTr ("Stop")

                    // Enabled when rendering is active
                    enabled: _renderer.active

                    // Stop rendering on click
                    onClicked: {
                        _renderer.stop ()
                    }
                }
            }
        }
    }
}
