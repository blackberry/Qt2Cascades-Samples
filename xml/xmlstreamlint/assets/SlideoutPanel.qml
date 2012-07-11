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

Container {
    id: root

    signal clicked()

    preferredHeight: 619

    Container {
        layout: DockLayout {}

        ImageView {
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
            }

            imageSource: "asset:///images/slideout_background.png"
        }

        Container {
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
            }

            layout: StackLayout {
                leftPadding: 20
                rightPadding: 20
            }

            // The standard ListView holding the HTML file selections
            ListView {
                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Fill
                    spaceQuota: 1
                }

                objectName: "listView"
                dataModel: _model

                listItemComponents: ListItemComponent {
                    type: "item"
                    StandardListItem {
                        description: ListItemData.title
                    }
                }

                // Set and parse the HTML file on selection
                onSelectionChanged: _app.setFileName (indexPath)
            }

            Label {
                topMargin: 30
                preferredHeight: 130

                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Center
                }

                text: _app.fileTitle

                textStyle {
                    alignment: TextAlignment.Center
                    base: SystemDefaults.TextStyles.BigText
                    color: Color.White
                }

                onTouch: {
                    if (event.isDown())
                        root.clicked()
                }
            }
        }

    }

    ImageView {
        layoutProperties: StackLayoutProperties {
            horizontalAlignment: HorizontalAlignment.Fill
        }

        imageSource: "asset:///images/shadow.png"
        translationY: -7
    }
}