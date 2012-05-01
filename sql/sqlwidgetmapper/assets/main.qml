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

// Page shows layout of TextField's bound to database data
Page {
    content: Container {
        layout: DockLayout {
        }
        
        // Container mainly for setting the background Color
        Container {
            background: Color.create ("#210a50")
            layout: StackLayout {
                leftPadding: 20
                rightPadding: 20
                topPadding: 20
                bottomPadding: 20
            }
            
            // Groups the label and corresponding TextField
            Container {
                layout: StackLayout {
                    layoutDirection: LayoutDirection.LeftToRight
                    bottomPadding: 20
                }
                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Fill
                }
                
                // A standard Label
                Label {
                    preferredWidth: 200
                    layoutProperties: StackLayoutProperties {
                        verticalAlignment: VerticalAlignment.Center
                    }
                    
                    text: qsTr ("First Name")
                    
                    // Defines custom Color
                    textStyle {
                        base: SystemDefaults.TextStyles.SmallText
                        color: Color.create ("#e3741a")
                    }
                }
                
                Container {
                    
                    // Standard TextField
                    TextField {
                        objectName: "firstNameField"
                        leftMargin: 10
                        preferredWidth: 800
                        
                        // Defines custom text Color
                        textStyle {
                            base: SystemDefaults.TextStyles.SmallText
                            color: Color.create ("#210a50")
                        }
                    }
                }
            }
            
            // Groups the label and corresponding TextField
            Container {
                layout: StackLayout {
                    layoutDirection: LayoutDirection.LeftToRight
                    bottomPadding: 20
                }
                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Fill
                }
                
                // A standard Label
                Label {
                    preferredWidth: 200
                    layoutProperties: StackLayoutProperties {
                        verticalAlignment: VerticalAlignment.Center
                    }
                                        
                    text: qsTr ("Last Name")
                    
                    // Defines custom text Color
                    textStyle {
                        base: SystemDefaults.TextStyles.SmallText
                        color: Color.create ("#e3741a")
                    }
                }
                Container {
                    
                    // A standard TextField
                    TextField {
                        objectName: "lastNameField"
                        preferredWidth: 800
                        
                        // Defines custom text Color
                        textStyle {
                            base: SystemDefaults.TextStyles.SmallText
                            color: Color.create ("#210a50")
                        }
                    }
                }
            }
            
            // Groups the label and corresponding TextArea
            Container {
                layout: StackLayout {
                    layoutDirection: LayoutDirection.LeftToRight
                    bottomPadding: 20
                }
                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Fill
                }
                
                // A standard Label
                Label {
                    preferredWidth: 200
                    layoutProperties: StackLayoutProperties {
                        verticalAlignment: VerticalAlignment.Center
                    }
                    
                    text: qsTr ("Quote")
                    
                    // Defines custom text Color
                    textStyle {
                        base: SystemDefaults.TextStyles.SmallText
                        color: Color.create ("#e3741a")
                    }
                }
                
                // A standard TextArea
                TextArea {
                    objectName: "quoteField"
                    preferredWidth: 800
                    preferredHeight: 350
                    
                    // Defines custom text Color
                    textStyle {
                        base: SystemDefaults.TextStyles.SmallText
                        color: Color.create ("#210a50")
                    }
                }
            }
            
            // Groups the application control Buttons
            Container {
                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Fill
                }
                layout: StackLayout {
                    layoutDirection: LayoutDirection.LeftToRight
                }
                
                // A standard Button
                Button {
                    layoutProperties: StackLayoutProperties {
                        verticalAlignment: VerticalAlignment.Center
                        spaceQuota: 1
                    }
                    
                    text: "Previous"
                    
                    // Go to previous data on click
                    onClicked: _dataMapper.toPrevious ()
                    
                    // Enable Button if there is previous data
                    enabled: _dataMapper.currentIndex != 0
                    
                }
                
                // A standard Label
                Label {
                    layoutProperties: StackLayoutProperties {
                        verticalAlignment: VerticalAlignment.Center
                        spaceQuota: 1
                    }
                    
                    text: (_dataMapper.currentIndex + 1) + "/" + _dataMapper.count
                    
                    // Defines custom text Color
                    textStyle {
                        alignment: TextAlignment.Center
                        color: Color.create ("#e3741a")
                    }
                }
                
                // A standard Button
                Button {
                    layoutProperties: StackLayoutProperties {
                        verticalAlignment: VerticalAlignment.Center
                        spaceQuota: 1
                    }
                    text: "Next"
                    
                    // Go to next data on click
                    onClicked: _dataMapper.toNext ()
                    
                    // Enable Button only if there is more data
                    enabled: (_dataMapper.currentIndex < (_dataMapper.count - 1))
                    
                }
            }
        }
    }
}
