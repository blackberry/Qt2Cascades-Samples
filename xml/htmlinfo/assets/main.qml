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


// Page shows different HTML file selections in a ListView that will be parsed using a xml
// reader and statistics about the file will be displayed
Page {
    content: Container {
        layout: DockLayout {
        }
        Container {
            layout: StackLayout {
                layoutDirection: LayoutDirection.LeftToRight
            }
            
            // Container for giving the ListView a background Color
            Container {
                background: Color.create ("#150790")
                
                // The standard ListView holding the HTML file selections
                ListView {
                    preferredWidth: 380
                    objectName: "listView"
                    dataModel: _model
                    listItemManager: _itemManager
                    
                    // Set and parse the HTML file on selection
                    onSelectionChanged: _app.setFileName (indexPath)
                }
            }
            Container {
                background: Color.create ("#eaefb8")
                layout: StackLayout {
                }
                layoutProperties: StackLayoutProperties {
                    verticalAlignment: VerticalAlignment.Fill
                }
                
                // A standard TextArea for displaying any parse errors
                TextArea {
                    preferredWidth: 680
                    preferredHeight: 360
                    editable: false
                    
                    // Make this Container visible only if there are errors
                    visible: _htmlInfo.error != ""
                    
                    text: _htmlInfo.error
                    
                    // Defines custom text style
                    textStyle {
                        base: SystemDefaults.TextStyles.SmallText
                        color: Color.DarkRed
                        alignment: TextAlignment.Center
                    }
                }
                Container {
                    // Container grouping HTML parsed output only visible if there were no errors
                    visible: _htmlInfo.error == ""
                    Container {
                        background: Color.create ("#7b9812")
                        layout: StackLayout {
                        }
                        layoutProperties: StackLayoutProperties {
                            horizontalAlignment: HorizontalAlignment.Fill
                            verticalAlignment: VerticalAlignment.Fill
                        }
                        
                        // A standard TextArea for displaying the HTML head title
                        TextArea {
                            preferredWidth: 900
                            preferredHeight: 125
                            backgroundVisible: false
                            
                            text: _htmlInfo.title
                            
                            // Defines text style
                            textStyle {
                                base: SystemDefaults.TextStyles.SmallText
                                fontWeight: FontWeight.Bold
                            }
                        }
                    }
                    
                    // Container grouping the Label and the corresponding output
                    Container {
                        background: Color.create ("#b0b38b")
                        layout: StackLayout {
                            layoutDirection: LayoutDirection.LeftToRight
                            leftPadding: 10
                        }
                        layoutProperties: StackLayoutProperties {
                            horizontalAlignment: HorizontalAlignment.Fill
                        }
                        
                        // A standard Label
                        Label {
                            maxWidth: 300
                            minWidth: maxWidth
                            
                            text: qsTr ("Number of paragraphs:")
                            textStyle {
                                base: SystemDefaults.TextStyles.SmallText
                                color: Color.Black
                            }
                        }
                        
                        // A standard Label displaying paragraphs output
                        Label {
                            text: _htmlInfo.paragraphs
                            textStyle {
                                base: SystemDefaults.TextStyles.SmallText
                                color: Color.Black
                            }
                        }
                    }
                    
                    // Container grouping the Label and the corresponding output
                    Container {
                        background: Color.create ("#c7cb9e")
                        layout: StackLayout {
                            layoutDirection: LayoutDirection.LeftToRight
                            leftPadding: 10
                        }
                        layoutProperties: StackLayoutProperties {
                            horizontalAlignment: HorizontalAlignment.Fill
                            verticalAlignment: VerticalAlignment.Fill
                        }
                        
                        // A standard Label
                        Label {
                            maxWidth: 300
                            minWidth: maxWidth
                            
                            text: qsTr ("Number of links:")
                            
                            // Defines text style
                            textStyle {
                                base: SystemDefaults.TextStyles.SmallText
                                color: Color.Black
                            }
                        }
                        
                        // A standard Label displaying number of links output
                        Label {
                            text: _htmlInfo.links
                            
                            // Defines text style
                            textStyle {
                                base: SystemDefaults.TextStyles.SmallText
                                color: Color.Black
                            }
                        }
                    }
                    
                    // Container grouping the Label and the corresponding output
                    Container {
                        background: Color.create ("#eaefb8")
                        layout: StackLayout {
                            leftPadding: 10
                        }
                        layoutProperties: DockLayoutProperties {
                            horizontalAlignment: HorizontalAlignment.Fill
                        }
                        
                        // A standard Label
                        Label {
                            text: qsTr ("Available links:")
                            textStyle {
                                base: SystemDefaults.TextStyles.SmallText
                                color: Color.Black
                            }
                        }
                        Container {
                            layout: StackLayout {
                                leftPadding: 30
                            }
                            
                            // A standard TextArea displaying link url's
                            TextArea {
                                preferredWidth: 850
                                preferredHeight: 500
                                editable: false
                                
                                text: _htmlInfo.linksContent
                                
                                // Defines text style
                                textStyle {
                                    base: SystemDefaults.TextStyles.SmallText
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
