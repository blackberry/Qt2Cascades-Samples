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

// This page shows how to create control buttons with a dynamically changing label
Page {

    // The root Container
    Container {
        layout: DockLayout {}

        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

        // A Container is used to gather visual items together.
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            preferredWidth: 1000

            //! [0]
            // A standard Label
            Label {
                horizontalAlignment: HorizontalAlignment.Center

                text: _sender.status

                // Defines text style with custom font size
                textStyle {
                    base: SystemDefaults.TextStyles.BodyText
                    color: Color.White
                    textAlign: TextAlign.Center
                }

                multiline: true
            }
            //! [0]

            // A Container for grouping the Label and TextField pair in a different Layout
            Container {
                horizontalAlignment: HorizontalAlignment.Center
                topMargin: 30

                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }

                // A standard Label
                Label {
                    verticalAlignment: VerticalAlignment.Center

                    text: qsTr ("TTL for multicast datagrams:")
                    textStyle {
                        base: SystemDefaults.TextStyles.BodyText
                        color: Color.White
                    }
                }

                //! [1]
                // A standard TextField
                TextField {
                    leftMargin: 10
                    preferredWidth: 200
                    text: _sender.ttl

                    // On text input store the text
                    onTextChanging: _sender.ttl = text
                }
                //! [1]
            }

            // A Container for grouping the control Buttons
            Container {
                horizontalAlignment: HorizontalAlignment.Center
                topMargin: 30
                leftPadding: 30
                rightPadding: 30

                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }

                //! [2]
                // A standard Button
                Button {
                    text: qsTr ("Start")

                    // Start the multicast on click
                    onClicked: {
                        enabled = false;
                        _sender.startSending ()
                    }
                }
                //! [2]

                // A standard Button
                Button {
                    leftMargin: 100
                    text: qsTr ("Quit")

                    // Quit the application on click
                    onClicked: _app.quit ()
                }
            }
        }
    }
}
