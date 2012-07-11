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

// Groups various visual nodes
Container {
    layout: StackLayout {
        leftPadding: 30
        rightPadding: 30
        topPadding: 30
    }
    objectName: "mainView"

//! [0]
    // Function tasked with the setting and translation of the Label's text
    function retranslate () {
        headerLabel.text = qsTr ("Language: %1").arg (qsTr ("English"));
        viewLabel.text = qsTr ("View");
        perspectiveLabel.text = qsTr ("Perspective");
        isometricLabel.text = qsTr ("Isometric");
        obliqueLabel.text = qsTr ("Oblique");
    }
//! [0]

    // A standard Label defining the language header
    Label {
        id: headerLabel
        topMargin: 10
        textStyle {
            color: Color.create ("#e3741a")
        }
    }

    // Container used as a visula divider line
    Container {
        background: Color.White
        preferredHeight: 2
        topMargin: 10
        bottomMargin: 10
        layoutProperties: StackLayoutProperties {
            horizontalAlignment: HorizontalAlignment.Fill
        }
    }

    // Groups the various Label's that are to be translated to selected language
    Container {
        layout: StackLayout {
            bottomPadding: 30
        }
        layoutProperties: StackLayoutProperties {
            horizontalAlignment: HorizontalAlignment.Fill
        }

        // A standard Label
        Label {
            id: viewLabel
            topMargin: 10
            layoutProperties: StackLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Fill
            }

            // Defines custom text style
            textStyle {
                base: SystemDefaults.TextStyles.TitleText
                alignment: TextAlignment.Center
                color: Color.White
                fontWeight: FontWeight.Bold
            }
        }

        // A standard Label
        Label {
            id: perspectiveLabel
            textStyle {
                color: Color.create ("#e3741a")
            }
        }

        // A standard Label
        Label {
            id: isometricLabel
            textStyle {
                color: Color.create ("#e3741a")
            }
        }

        // A standard Label
        Label {
            id: obliqueLabel
            textStyle {
                color: Color.create ("#e3741a")
            }
        }
    }

    Container {
        layout: StackLayout {
            bottomPadding: 20
        }

//! [1]
        // A standard TextArea
        TextArea {
            id: textArea
            objectName: "textArea"
//! [1]
            preferredHeight: 200
            layoutProperties: StackLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Fill
            }

            // Defines custom text Color
            textStyle {
                color: Color.create ("#e3741a")
            }
        }
    }
}
