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
    // A container is used to gather visual items together.
    Container {
        layout: DockLayout {}

        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            // Defines a TextStyleDefinition that can be used in each control.
            attachedObjects: [
                TextStyleDefinition {
                    id: tsd
                    color: Color.White
                    base: SystemDefaults.TextStyles.BodyText
                    textAlign: TextAlign.Center
                }
            ]

            Container {
                horizontalAlignment: HorizontalAlignment.Fill

                // A standard Label
                Label {
                    id: title

                    horizontalAlignment: HorizontalAlignment.Fill

                    text: qsTr ("Ready to broadcast datagrams on port 45454")
                    textStyle {
                        color: Color.White
                        base: tsd.style
                    }
                }

//! [0]
                // A standard Label, displays sending data
                Label {
                    horizontalAlignment: HorizontalAlignment.Center

                    text: _sender.status
                    textStyle {
                        color: Color.White
                        fontWeight: FontWeight.Bold
                        fontSize: FontSize.Medium
                    }

                    // Plays scale animation upon sending the data
                    animations: [
                        SequentialAnimation {
                            id: animTextSize
                            ScaleTransition {
                                toX: 1.5
                                toY: 1.5
                                duration: 300
                            }
                            ScaleTransition {
                                toX: 1.0
                                toY: 1.0
                                duration: 300
                            }
                        }
                    ]

                    // Play animation when text changes
                    onTextChanged: animTextSize.play ()
                }
            }
//! [0]

            // The control Container, contains the buttons to start and quit application.
            Container {
                topMargin: 30
                horizontalAlignment: HorizontalAlignment.Center
                leftPadding: 30
                rightPadding: 30

                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }

//! [1]
                // A standard button
                Button {
                    text: qsTr ("Start")

                    // Start broadcasting datagrams on click
                    onClicked: {
                        title.text = qsTr ("Now broadcasting datagram")
                        enabled = false;
                        _sender.startBroadcasting ()
                    }
                }
//! [1]

                // A standard button
                Button {
                    leftMargin: 100
                    text: qsTr ("Quit")

                    // Quit application on click
                    onClicked: _app.quit ()
                }
            }
        }
    }
}
