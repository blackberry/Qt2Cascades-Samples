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

// Container grouping all visual nodes for the main view
Container {
    layout: StackLayout {
        leftPadding: 30
        topPadding: 30
        rightPadding: 30
    }

    // TextStyleDefinition to be used in different AbstractTextControl's to achieve same text look.
    attachedObjects: [
        TextStyleDefinition {
            id: labelTsd
            base: SystemDefaults.TextStyles.SmallText
            color: Color.White
        }
    ]

    // A standard Label
    Label {
        text: qsTr ("Host name:")
        layoutProperties: StackLayoutProperties {
            verticalAlignment: VerticalAlignment.Center
            spaceQuota: -1
        }
        textStyle {
            base: labelTsd.style
        }
    }

    // A Container to group the url and its corresponding TextField
    Container {
        topMargin: 10
        bottomMargin: 10

        layoutProperties: StackLayoutProperties {
            horizontalAlignment: HorizontalAlignment.Fill
        }

        layout: StackLayout {
            layoutDirection: LayoutDirection.LeftToRight
        }

//! [0]
        // A standard TextField
        TextField {
            leftMargin: 10
            rightMargin: 5
            layoutProperties: StackLayoutProperties {
                verticalAlignment: VerticalAlignment.Center
                spaceQuota: 1
            }

            // Save url on input
            onTextChanging: _sslClient.hostName = text
        }
//! [0]

        // A standard ImageView
        ImageView {
            imageSource: "asset:///encrypted.png"
            preferredWidth: 32
            preferredHeight: 32
            layoutProperties: StackLayoutProperties {
                verticalAlignment: VerticalAlignment.Center
                spaceQuota: -1
            }

            // Visibility control
            visible: _sslClient.sessionActive

            // Display certificate info view on touch
            onTouch: _certificateInfoControl.visible = true
        }
    }

    // A standard Label
    Label {
        text: qsTr ("Port:")
        textStyle {
            base: labelTsd.style
        }
    }

    // A standard TextField
    TextField {
        // Save port on input
        onTextChanging: _sslClient.port = text
    }

//! [1]
    // A standard Button
    Button {
        topMargin: 30
        bottomMargin: 10
        maxHeight: 100
        layoutProperties: StackLayoutProperties {
            horizontalAlignment: HorizontalAlignment.Fill
        }

        text: qsTr ("Connect to host")

        // Connect to url on click
        onClicked: _sslClient.secureConnect ()
    }
//! [1]

    // Groups the nodes for displaying connection details and connection communication controls.
    Container {
        leftMargin: 20
        topMargin: 20
        layoutProperties: StackLayoutProperties {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            spaceQuota: 1
        }

        // A standard Label
        Label {
            layoutProperties: StackLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Fill
            }

            text: qsTr ("Active session")
            textStyle {
                base: labelTsd.style
                alignment: TextAlignment.Center
            }
        }

        // A standard Label
        Label {
            text: qsTr ("Cryptographic Cipher:")
            minWidth: 280
            textStyle {
                base: SystemDefaults.TextStyles.SmallText
            }
        }

        // A standard Label
        Label {
            layoutProperties: StackLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center
            }

            // Displays cipher information upon successful connection
            text: _sslClient.cipher
            textStyle {
                base: SystemDefaults.TextStyles.SmallText
                alignment: TextAlignment.Center
                color: Color.Red
            }
        }

        // A standard TextArea
        TextArea {
            layoutProperties: StackLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                spaceQuota: 7
            }

            // Displays connection response after sending data
            text: _sslClient.response
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                color: Color.Green
            }
            hintText: ""
            editable: false
        }

        // Groups the input and Button for data sending
        Container {
            topMargin: 10
            bottomMargin: 10
            layoutProperties: StackLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Fill
                spaceQuota: 2
            }
            layout: StackLayout {
                layoutDirection: LayoutDirection.LeftToRight
            }

            // Enable these controls only when connection has been established
            enabled: _sslClient.sessionActive

            // A standard Label
            Label {
                text: qsTr ("Input:")

                layoutProperties: StackLayoutProperties {
                    verticalAlignment: VerticalAlignment.Center
                    spaceQuota: -1
                }

                textStyle {
                    base: SystemDefaults.TextStyles.SmallText
                }
            }

            // A standard TextField
            TextField {
                id: inputField
                leftMargin: 10

                layoutProperties: StackLayoutProperties {
                    verticalAlignment: VerticalAlignment.Center
                    spaceQuota: 1
                }
            }

            // A standard Button
            Button {
                leftMargin: 10
                text: qsTr ("Send")
                preferredWidth: 250

                layoutProperties: StackLayoutProperties {
                    verticalAlignment: VerticalAlignment.Center
                    spaceQuota: -1
                }

                // Send input data on click
                onClicked: {
                    _sslClient.sendData (inputField.text)
                    inputField.text = ""
                }
            }
        }
    }
}
