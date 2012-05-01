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

// Container grouping all visual nodes for the initial view
Container {
    topMargin: 10
    leftMargin: 10
    rightMargin: 10
    bottomMargin: 10
    
    // Groups the ftp url TextField and connection control Buttons
    Container {
        layout: StackLayout {
            layoutDirection: LayoutDirection.LeftToRight
        }
        
        // A standard Label
        Label {
            text: qsTr ("Ftp server:")
            layoutProperties: StackLayoutProperties {
                spaceQuota: -1
            }
        }
        
        // A standard TextField
        TextField {
            leftMargin: 5
            layoutProperties: StackLayoutProperties {
                spaceQuota: 3
            }
            text: _downloader.url
            preferredWidth: 400
            
            // Save ftp url on input
            onTextChanging: _downloader.url = text
        }
        
        // A standard Button
        Button {
            leftMargin: 15
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            text: qsTr ("Up")
            
            // Enable control upon successful connection
            enabled: _downloader.parentDirectoryAvailable
            
            // Move up to parent folder in directory tree
            onClicked: _downloader.cdToParent ()
        }
        
        // A standard Button
        Button {
            leftMargin: 15
            layoutProperties: StackLayoutProperties {
                spaceQuota: 2
            }
            
            // Button text depends on state of connection (connect/disconnect)
            text: _downloader.connectLabel
            enabled: _downloader.connectPossible
            
            // Deppending on state, connect/disconnect to/from ftp server
            onClicked: _downloader.connectOrDisconnect ()
        }
    }
    
    // Container to change background color of ListView
    Container {
        layoutProperties: StackLayoutProperties {
            horizontalAlignment: HorizontalAlignment.Fill
            spaceQuota: 3
        }
        
        // A standard ListView
        ListView {
            topMargin: 10
            dataModel: _model
            listItemManager: _itemManager
            enabled: _downloader.selectionPossible
            
            // Open folder or highlight file on selection
            onSelectionChanged: _downloader.processItem (indexPath, selected)
        }
    }
    
    // A standard Label
    Label {
        topMargin: 10
        preferredWidth: 1000
        layoutProperties: StackLayoutProperties {
            horizontalAlignment: HorizontalAlignment.Fill
        }
        
        text: _downloader.statusText
        
        // Define custom text color
        textStyle {
            base: SystemDefaults.TextStyles.SmallText
            size: 25
        }
    }
    
    // Groups the control Buttons
    Container {
        layoutProperties: StackLayoutProperties {
            horizontalAlignment: HorizontalAlignment.Center
            spaceQuota: 1
        }
        layout: StackLayout {
            layoutDirection: LayoutDirection.LeftToRight
        }
        
        // A standard Button
        Button {
            text: qsTr ("Download")
            
            // Enable Button upon successful connection
            enabled: _downloader.downloadPossible
            
            // Download and save on click
            onClicked: _downloader.downloadFile ()
        }
        
        // A standard Button
        Button {
            leftMargin: 25
            text: qsTr ("Quit")
            
            // Quit application on click
            onClicked: Qt.quit ()
        }
    }
}
