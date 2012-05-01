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

// Page shows the loading and saving of XBEL files using the streaming parser
Page {
    content: Container {
        layout: DockLayout {
        }
        Container {
            id: root
            background: Color.LightGray
            layout: StackLayout {
            }
            
            // Container grouping application Button controls
            Container {
                id: menuContainer
                background: Color.DarkGray
                layout: StackLayout {
                    layoutDirection: LayoutDirection.LeftToRight
                    topPadding: 10
                    bottomPadding: 10
                }
                layoutProperties: DockLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VertialAlignment.Top
                }
                
                // A standard Button
                Button {
                    id: frank
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                    
                    text: "Frank"
                    
                    // Load the selected xbel file on click
                    onClicked: {
                        jennifer.opacity = 0.5;
                        save.opacity = 0.5;
                        frank.opacity = 1.0;
                        _app.load ("frank.xbel");
                    }
                }
                
                // A standard Button
                Button {
                    id: jennifer
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                    
                    text: "Jennifer"
                    
                    // Load the selected xbel file on click
                    onClicked: {
                        frank.opacity = 0.5;
                        save.opacity = 0.5;
                        jennifer.opacity = 1.0;
                        _app.load ("jennifer.xbel");
                    }
                }
                
                // A standard Button
                Button {
                    id: save
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                    
                    text: "Save"
                    
                    // Save the changes to a temporary xbel file in the
                    // application tmp/ directory
                    onClicked: {
                        frank.opacity = 0.5;
                        jennifer.opacity = 0.5;
                        save.opacity = 1.0;
                        _app.save ();
                    }
                }
            }
            Container {
                background: Color.DarkGray
                layout: StackLayout {
                    layoutDirection: LayoutDirection.LeftToRight
                    topPadding: 10
                    bottomPadding: 10
                }
                layoutProperties: DockLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VertialAlignment.Top
                }
                
                // A standard Label for displaying the action status
                Label {
                    layoutProperties: StackLayoutProperties {
                        verticalAlignment: VerticalAlignment.Center
                        spaceQuota: 1
                    }
                    
                    text: "Status: " + _app.status
                    textStyle {
                        base: SystemDefaults.TextStyles.SmallText
                        color: Color.Black
                    }
                }
            }
            
            // Container for displaying the loaded XBEL output
            Container {
                objectName: "treeContainer"
                scrollMode: ScrollMode.Vertical
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
            }
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
            }
        }
    }
}
