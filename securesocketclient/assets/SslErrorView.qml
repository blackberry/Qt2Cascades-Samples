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


// Container grouping all visual nodes for the ssl error view
Container {
    leftMargin: 10
    topMargin: 10
    rightMargin: 10
    bottomMargin: 10

    background: Color.White

    // A standard TextArea
    Label {
        horizontalAlignment: HorizontalAlignment.Fill
        text: qsTr ("Warning: One or more errors with this connection prevent validating the authenticity of the host you are connecting to. Please review the following list of errors, and click Ignore to continue, or Cancel to abort the connection.")
        multiline: true
    }

    // A standard TextArea
    TextArea {
        horizontalAlignment: HorizontalAlignment.Fill
        topMargin: 10
        preferredHeight: 400

        // Displays the ssl error message
        text: _sslErrorControl.message
    }

    // A standard Button
    Button {
        horizontalAlignment: HorizontalAlignment.Fill

        text: qsTr ("View Certificate Chain")

        // Display certificate invo on click
        onClicked: _sslErrorControl.viewCertificateChain ()
    }

    // Groups the views control Buttons
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        topMargin: 10

        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }

        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }

        // A standard Button
        Button {
            text: qsTr ("Ignore")

            // Dismiss view and error on click and continue
            onClicked: _sslErrorControl.ignore ()
        }

        // A standard Button
        Button {
            text: qsTr ("Cancel")

            // Hide view on click
            onClicked: _sslErrorControl.cancel ()
        }
    }
}
