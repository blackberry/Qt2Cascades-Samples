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

// Container for the Downloader visual nodes
Container {
    leftMargin: 10
    topMargin: 10
    rightMargin: 10
    bottomMargin: 10
    
    // Container grouping the url label and input
    Container {
        layout: StackLayout {
            layoutDirection: LayoutDirection.LeftToRight
        }
        
        // A standard Label
        Label {
            rightMargin: 10
            
            text: qsTr ("URL:")
            layoutProperties: StackLayoutProperties {
                verticalAlignment: VerticalAlignment.Center
            }
        }
        
        // A standard TextField
        TextField {
            id: urlEdit
            preferredWidth: 900
            
            // Default url to use
            text: "http://qt-project.org/"
            textStyle {
                base: SystemDefaults.TextStyles.SmallText
            }
            //text: "http://www.pagetutor.com/keeper/mystash/secretstuff.html" // For HTTP authentication example (username is jimmy, password is page)
            //text: "https://qt.nokia.com" // For site with SSL errors (untrusted certificate)
            //text: "http://www.example.com" // For site which redirects
            enabled: _downloader.startDownloadPossible
        }
    }
    
    // A standard Label for the download status
    Label {
        topMargin: 10
        layoutProperties: StackLayoutProperties {
            horizontalAlignment: HorizontalAlignment.Fill
        }
        
        text: _downloader.statusText
        textStyle {
            base: SystemDefaults.TextStyles.SmallText
        }
    }
    
    // Container for the application controls
    Container {
        topMargin: 10
        layout: StackLayout {
            layoutDirection: LayoutDirection.LeftToRight
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
