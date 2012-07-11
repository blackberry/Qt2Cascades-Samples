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

// Page displays layout of TextField's, Label's, CheckBox's and DopDown's to run a user
// defined regular expression on a string and display the detailed matched results
Page {

    // The root container, used to set the background Color
    content: Container {
        layout: DockLayout {}

        // The background image
        ImageView {
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
            }

            imageSource: "asset:///images/background.png"
        }

        Container {
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
            }

            // A standard Label for the application header
            Label {
                preferredHeight: 200
                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Center
                }

                text: "Regular Expression Example"

                // Defines custom text style
                textStyle {
                    base: SystemDefaults.TextStyles.TitleText
                    fontWeight: FontWeight.Bold
                    color: Color.Black
                }
            }

            // Enable/Disable vertical scroll mode
            ScrollView {
                scrollViewProperties {
                    scrollMode: ScrollMode.Vertical
                }

                Container {

                    // The TextStyleDefinition's to be used in AbstractTextControl's
                    attachedObjects: [

                        // Defines custom text style definition
                        TextStyleDefinition {
                            id: tsd
                            base: SystemDefaults.TextStyles.BodyText
                            color: Color.Black
                        },

                        // Defines custom text style definition
                        TextStyleDefinition {
                            id: tsh
                            base: tsd.style
                            fontWeight: FontWeight.Bold
                        }
                    ]

                    layout: StackLayout {
                        layoutDirection: LayoutDirection.TopToBottom
                        leftPadding: 20
                        rightPadding: 20
                    }

                    // A standard Label
                    Label {
                        text: "Inputs:"
                        textStyle {
                            base: SystemDefaults.TextStyles.TitleText
                            fontWeight: FontWeight.Bold
                            color: Color.Black
                        }
                    }

                    // A standard Label
                    Label {
                        leftMargin: 10
                        topMargin: 10

                        text: "Pattern:"
                        textStyle {
                            base: tsh.style
                        }
                    }

//! [0]
                    // A standard TextField for the regular expression pattern
                    TextField {
                        leftMargin: 10
                        topMargin: 10
                        preferredWidth: 500
                        text: _regexp.pattern
                        onTextChanging: _regexp.pattern = text
                        textStyle {
                            base: tsd.style
                        }
                    }
//! [0]


                    // A standard Label
                    Label {
                        topMargin: 10

                        text: "Escaped Pattern:"
                        textStyle {
                            base: tsh.style
                        }
                    }

//! [1]
                    // A standard Label
                    Label {

                        // Displays the regular expression pattern input from above TextField
                        text: _regexp.escapedPattern
                        textStyle {
                            base: tsd.style
                        }
                    }
//! [1]

                    // Drop down for selecting various pattern syntax
                    DropDown {
                        title: "Pattern Syntax:"
                        onSelectedIndexChanged: _app.setPatternSyntax (selectedIndex)
                        Option {
                            text: "Regular expression v1"
                            selected: true
                        }
                        Option {
                            text: "Regular expression v2"
                        }
                        Option {
                            text: "Wildcard"
                        }
                        Option {
                            text: "Fixed string"
                        }
                        Option {
                            text: "W3C Xml Schema 1.1"
                        }
                    }

                    // Container grouping the CheckBox's and their corresponding Label's
                    Container {
                        layout: StackLayout {
                            layoutDirection: LayoutDirection.LeftToRight
                        }

                        Container {
                            topMargin: 10
                            layout: StackLayout {
                                layoutDirection: LayoutDirection.LeftToRight
                            }

                            // A standard CheckBox
                            CheckBox {
                                // Default check
                                checked: true

                                // Save the boolean value upon being checked
                                onCheckedChanged: _regexp.caseSensitive = checked
                            }

                            // A standard Label for the checkbox
                            Label {
                                leftMargin: 10

                                text: "Case Sensitive"
                                textStyle {
                                    base: tsh.style
                                }
                            }
                        }

                        Container {
                            topMargin: 10
                            leftMargin: 25
                            layout: StackLayout {
                                layoutDirection: LayoutDirection.LeftToRight
                            }

                            // A standard CheckBox
                            CheckBox {

                                // Save the boolean value upon being checked
                                onCheckedChanged: _regexp.minimal = checked
                            }

                            // A standard Label for the checkbox
                            Label {
                                leftMargin: 10
                                text: "Minimal"
                                textStyle {
                                    base: tsh.style
                                }
                            }
                        }
                    }

                    // A standard Label
                    Label {
                        topMargin: 10

                        text: "Text:"
                        textStyle {
                            base: tsh.style
                        }
                    }

                    // A standard TextField for the input string to run the regular
                    // expression against
                    TextField {
                        preferredWidth: 500

                        text: _regexp.text
                        textStyle {
                            base: tsd.style
                        }

                        // Save the text on input
                        onTextChanging: _regexp.text = text
                    }

                    // A standard Label
                    Label {
                        text: "Outputs:"
                        textStyle {
                            base: SystemDefaults.TextStyles.TitleText
                            fontWeight: FontWeight.Bold
                            color: Color.Black
                        }
                    }

                    Container {
                        layout: StackLayout {
                            layoutDirection: LayoutDirection.LeftToRight
                        }

                        // A standard Label
                        Label {
                            topMargin: 10

                            text: "Index of Match:"
                            textStyle {
                                base: tsh.style
                            }
                        }

                        // A standard Label for the index of match output
                        Label {
                            text: _regexp.matchIndex + ""
                            textStyle {
                                base: tsd.style
                            }
                        }
                    }

                    Container {
                        layout: StackLayout {
                            layoutDirection: LayoutDirection.LeftToRight
                        }

                        // A standard Label
                        Label {
                            topMargin: 10

                            text: "Matched Length:"
                            textStyle {
                                base: tsh.style
                            }
                        }

                        // A standard Label for the matched length output
                        Label {
                            text: _regexp.matchLength + ""
                            textStyle {
                                base: tsd.style
                            }
                        }
                    }

                    // A standard Label
                    Label {
                        topMargin: 10

                        // Enabled if output available
                        enabled: _regexp.capture0Available

                        text: "Match:"
                        textStyle {
                            base: tsh.style
                        }
                    }

                    // A standard Label for the match output
                    Label {

                        // Enabled if output available
                        enabled: _regexp.capture0Available

                        text: _regexp.capture0Text
                        textStyle {
                            base: tsd.style
                        }
                    }

                    // A standard Label
                    Label {
                        topMargin: 10

                        // Enabled if output available
                        enabled: _regexp.capture1Available

                        text: "Capture 1:"
                        textStyle {
                            base: tsh.style
                        }
                    }

                    // A standard Label for the capture 1 output
                    Label {

                        // Enabled if output available
                        enabled: _regexp.capture1Available

                        text: _regexp.capture1Text
                        textStyle {
                            base: tsd.style
                        }
                    }

                    // A standard Label
                    Label {
                        topMargin: 10

                        // Enabled if output available
                        enabled: _regexp.capture2Available

                        text: "Capture 2:"
                        textStyle {
                            base: tsh.style
                        }
                    }

                    // A standard Label for the capture 2 output
                    Label {

                        // Enabled if output available
                        enabled: _regexp.capture2Available

                        text: _regexp.capture2Text
                        textStyle {
                            base: tsd.style
                        }
                    }

                    // A standard Label
                    Label {
                        topMargin: 10

                        // Enabled if output available
                        enabled: _regexp.capture3Available

                        text: "Capture 3:"
                        textStyle {
                            base: tsh.style
                        }
                    }

                    // A standard Label for the capture 3 output
                    Label {

                        // Enabled if output available
                        enabled: _regexp.capture3Available

                        text: _regexp.capture3Text
                        textStyle {
                            base: tsd.style
                        }
                    }

                    // A standard Label
                    Label {
                        topMargin: 10

                        // Enabled if output available
                        enabled: _regexp.capture4Available

                        text: "Capture 4:"
                        textStyle {
                            base: tsh.style
                        }
                    }

                    // A standard Label for the capture 4 output
                    Label {

                        // Enabled if output available
                        enabled: _regexp.capture4Available

                        text: _regexp.capture4Text
                        textStyle {
                            base: tsd.style
                        }
                    }

                    // A standard Label
                    Label {
                        topMargin: 10

                        // Enabled if output available
                        enabled: _regexp.capture5Available

                        text: "Capture 5:"
                        textStyle {
                            base: tsh.style
                        }
                    }

                    // A standard Label for the capture 5 output
                    Label {

                        // Enabled if output available
                        enabled: _regexp.capture5Available

                        text: _regexp.capture5Text
                        textStyle {
                            base: tsd.style
                        }
                    }
                }
            }
        }
    }
}
