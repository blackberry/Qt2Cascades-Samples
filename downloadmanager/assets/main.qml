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

// This page shows how to layout various standard visual components along with a custom component.
Page {

    // The root Container
    Container {
        layout: DockLayout {}

        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            leftPadding: 30
            topPadding: 20
            rightPadding: 30

            // A Container to group the url TextField with its download Button
            Container {
//! [0]
                // A standard TextField for the url address
                TextField {
                    id: urlField

                    horizontalAlignment: HorizontalAlignment.Fill

                    hintText: qsTr("Enter URL to download")
                    text: "http://www.winkde.org/pub/kde/ports/win32/installer/kdewin-installer-gui-latest.exe"

                    // Disable the control button upon text input
                    onTextChanging: {
                        downloadButton.enabled = (text != "")
                    }
                }
//! [0]

//! [1]
                // A standard button
                Button {
                    id: downloadButton

                    horizontalAlignment: HorizontalAlignment.Center
                    topMargin: 50

                    text: qsTr("Download")

                    // Start download from url on click
                    onClicked: _manager.downloadUrl (urlField.text)
                }
//! [1]
            }

//! [2]
            // Custom Container for displaying download progress as a bar
            ProgressBar {
                topMargin: 10
                leftMargin: 10

                total: _manager.progressTotal
                value: _manager.progressValue
                message: _manager.progressMessage
            }
//! [2]

//! [3]
            // A standard Label
            Label {
                topMargin: 10
                leftMargin: 10

                text: qsTr("Active Downloads: ") + (_manager.activeDownloads == 0 ? "none" : _manager.activeDownloads)
                textStyle.base: SystemDefaults.TextStyles.BodyText
                textStyle.color: Color.Black
            }
//! [3]

            // A standard Label
            Label {
                topMargin: 10
                leftMargin: 10

                text: qsTr("Status:")
                textStyle.base: SystemDefaults.TextStyles.BodyText
                textStyle.color: Color.Black
            }

//! [4]
            // A standard TextArea for the download status output
            TextArea {
                preferredWidth: 900
                preferredHeight: 145

                backgroundVisible: false
                editable: false

                text: _manager.statusMessage

                // Defines text style with custom Color
                textStyle {
                    base: SystemDefaults.TextStyles.BodyText
                    color: Color.create("#ff509d4c")
                }
            }
//! [4]

            // A standard Label
            Label {
                leftMargin: 10

                text: qsTr ("Errors:")
                textStyle.base: SystemDefaults.TextStyles.BodyText
                textStyle.color: Color.Black
            }

//! [5]
            // A standard TextArea for displaying error output
            TextArea {
                leftMargin: 10
                preferredWidth: 900
                preferredHeight: 125

                backgroundVisible: false
                editable: false

                text: _manager.errorMessage

                // Defines a text style with custom Color
                textStyle {
                    base: SystemDefaults.TextStyles.SmallText
                    color: Color.create("#ffbc3434")
                }
            }
//! [5]
        }
    }
}
