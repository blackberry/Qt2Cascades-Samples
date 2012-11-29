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

// Page shows the loading and saving of XBEL files using the DOM parser
Page {
    Container {
        layout: DockLayout {}

        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

        Container {
            id: root

            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            // Container grouping application Button controls
            Container {
                id: menuContainer

                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Top

                leftPadding: 10
                topPadding: 10
                rightPadding: 10
                bottomPadding: 10

                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }

//! [0]
                // A standard Button
                Button {
                    id: frank
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }

                    text: qsTr ("Frank")

                    // Load the selected xbel file on click
                    onClicked: {
                        jennifer.opacity = 0.5;
                        save.opacity = 0.5;
                        frank.opacity = 1.0;
                        _app.load ("frank.xbel");
                    }
                }
//! [0]

                // A standard Button
                Button {
                    id: jennifer
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }

                    text: qsTr ("Jennifer")

                    // Load the selected xbel file on click
                    onClicked: {
                        frank.opacity = 0.5;
                        save.opacity = 0.5;
                        jennifer.opacity = 1.0;
                        _app.load ("jennifer.xbel");
                    }
                }

//! [1]
                // A standard Button
                Button {
                    id: save
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }

                    text: qsTr ("Save")

                    // Save the changes to a temporary xbel file in the
                    // application tmp/ directory
                    onClicked: {
                        frank.opacity = 0.5;
                        jennifer.opacity = 0.5;
                        save.opacity = 1.0;
                        _app.save ();
                    }
                }
//! [1]
            }

            Container {
                background: Color.DarkGray

                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Top

                leftPadding: 10
                rightPadding: 10
                bottomPadding: 10

                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }

                // A standard Label for displaying the action status
                Label {
                    verticalAlignment: VerticalAlignment.Center

                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }

                    text: qsTr ("Status: %1").arg(_app.status)
                    textStyle {
                        base: SystemDefaults.TextStyles.SmallText
                        color: Color.Black
                    }
                }
            }

//! [2]
            // Container for displaying the loaded XBEL output
            ScrollView {
                topMargin: 10
                scrollViewProperties {
                    scrollMode: ScrollMode.Vertical
                }
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
                Container {
                    objectName: "treeContainer"

                    leftPadding: 10
                    rightPadding: 10
                    bottomPadding: 10
                }
            }
//! [2]
        }
    }
}
