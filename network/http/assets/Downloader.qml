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

//! [1]
import bb.cascades 1.0

// Container for the Downloader visual nodes
Container {
    leftPadding: 30
    topPadding: 20
    rightPadding: 30
    bottomPadding: 30

    // Container grouping the url label and input
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }

        // A standard Label
        Label {
            verticalAlignment: VerticalAlignment.Center
            rightMargin: 10

            layoutProperties: StackLayoutProperties {
                spaceQuota: -1
            }

            text: qsTr ("URL:")
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                color: Color.White
            }
        }
// ![0]
        // A standard TextField
        TextField {
            id: urlEdit

            verticalAlignment: VerticalAlignment.Center

            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }

            // Default url to use
            text: "http://qt-project.org/"
            //text: "http://www.pagetutor.com/keeper/mystash/secretstuff.html" // For HTTP authentication example (username is jimmy, password is page)
            //text: "https://qt.nokia.com" // For site with SSL errors (untrusted certificate)
            //text: "http://www.example.com" // For site which redirects

            textStyle {
                base: SystemDefaults.TextStyles.SmallText
            }

            enabled: _downloader.startDownloadPossible
        }
    }
// ![0]

    // A standard TextField for the download status
    TextField {
        horizontalAlignment: HorizontalAlignment.Fill
        topMargin: 50

        preferredHeight: 200

        enabled: false
        backgroundVisible: false

        text: _downloader.statusText
        textStyle {
            base: SystemDefaults.TextStyles.SmallText
            color: Color.Black
        }
    }

    // Container for the application controls
    Container {
        horizontalAlignment: HorizontalAlignment.Center
        topMargin: 30

        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }

        // A standard Button
        Button {
            rightMargin: 20

            text: qsTr ("Download")

            // Enable Button after text verification and when startDownloadPossible is set
            enabled: _downloader.startDownloadPossible && urlEdit.text.length > 0

            // Start the download on click
            onClicked: _downloader.startDownload (urlEdit.text)
        }

        // A standard Button
        Button {
            text: qsTr ("Quit")

            // Quit application on click
            onClicked: Qt.quit ()
        }
    }
}
//! [1]
