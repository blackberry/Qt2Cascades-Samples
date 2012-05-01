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

// Page shows different HTML file selections in a ListView that will be parsed using a xml stream
// reader and validates it for proper xml syntax
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
                    objectName: "listView"
                    leftMargin: 10
                    topMargin: 10
                    bottomMargin: 10
                    dataModel: _model
                    listItemManager: _itemManager
                    preferredWidth: 380
                    
                    // Set and parse the HTML file on selection
                    onSelectionChanged: _app.setFileName (indexPath)
                }
            }
            Container {
                layoutProperties: StackLayoutProperties {
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                }
                Container {
                    background: Color.create ("#7b9812")
                    
                    // A standard TextArea for displaying any xml parse errors
                    TextArea {
                        leftMargin: 10
                        preferredWidth: 900
                        preferredHeight: 145
                        editable: false
                        
                        layoutProperties: StackLayoutProperties {
                            horizontalAlignment: HorizontalAlignment.Fill
                        }
                        
                        text: _xmlStreamLint.result
                        
                        // Defines text style
                        textStyle {
                            base: SystemDefaults.TextStyles.SmallText
                            color: Color.Black
                        }
                    }
                }
                
                Container {
                    background: Color.create ("#e7f6b1")
                    
                    // A standard TextArea for displaying the HTML source
                    TextArea {
                        leftMargin: 10
                        editable: false
                        preferredWidth: 900
                        preferredHeight: 600
                        layoutProperties: StackLayoutProperties {
                            horizontalAlignment: HorizontalAlignment.Fill
                        }
                        
                        text: _xmlStreamLint.output
                        textStyle {
                            base: SystemDefaults.TextStyles.SmallText
                            color: Color.DarkBlue
                        }
                        
                    }
                }
            }
        }
    }
}
