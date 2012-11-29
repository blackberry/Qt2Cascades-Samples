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

Page {
    Container {
        layout: DockLayout {}

        // The background image
        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: verticalAlignment.Fill

            imageSource: "asset:///images/background.png"
        }

        // The top-level Container that contains the Maze and the control elements
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: verticalAlignment.Fill

            leftPadding: 30
            topPadding: 30
            rightPadding: 30
            bottomPadding: 30

            // The upper part of the screen
            Container {
                horizontalAlignment: HorizontalAlignment.Center

                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }

                layout: DockLayout {}

//! [0]
                // The maze board
                Maze {
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: verticalAlignment.Center
                }
//! [0]

                // The script error overlay text
                TextArea {
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: verticalAlignment.Center

                    preferredWidth: 400
                    preferredHeight: 400
                    opacity: _gameController.scriptError == "" ? 0.0 : 1.0
                    backgroundVisible: false

                    text: _gameController.scriptError

                    textStyle {
                        base: SystemDefaults.TextStyles.BigText
                        color: Color.White
                        textAlign: TextAlign.Center
                    }
                }
            }

            // The lower part of the screen
            Container {
                horizontalAlignment: HorizontalAlignment.Fill

                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }

                DropDown {
                    horizontalAlignment: HorizontalAlignment.Center

                    title: qsTr ("Scripts")

                    Option {
                        text: qsTr ("Simple moves")
                        onSelectedChanged: {
                            if (selected == true) {
                                scriptContent.text = "board.reset()\n" +
                                                     "player.turnLeft()\n" +
                                                     "player.go()\n" +
                                                     "player.turnRight()\n" +
                                                     "player.go()\n" +
                                                     "player.go()\n" +
                                                     "player.go()\n" +
                                                     "player.turnLeft()\n"
                            }
                        }
                    }

                    Option {
                        text: qsTr ("Random maze")
                        onSelectedChanged: {
                            if (selected == true) {
                                scriptContent.text = "board.staticBlockDistribution = false\n" +
                                                     "board.reset()\n" +
                                                     "board.staticBlockDistribution = true\n" +
                                                     "player.reset()\n"
                            }
                        }
                    }

                    Option {
                        text: qsTr ("New Player")
                        onSelectedChanged: {
                            if (selected == true) {
                                scriptContent.text = "board.reset()\n" +
                                                     "player.turnLeft()\n" +
                                                     "player.go()\n" +
                                                     "player.turnRight()\n" +
                                                     "player.go()\n" +
                                                     "var p = new Player(board)\n" +
                                                     "p.turnLeft()\n" +
                                                     "p.go()\n" +
                                                     "p.turnRight()\n" +
                                                     "p.go()\n" +
                                                     "p.go()\n" +
                                                     "p.go()"
                            }
                        }
                    }

                    Option {
                        text: qsTr ("Reset Engine")
                        onSelectedChanged: {
                            if (selected == true) {
                                scriptContent.text = "controller.reset()"
                            }
                        }
                    }
                }

//! [1]
                TextArea {
                    id: scriptContent

                    horizontalAlignment: HorizontalAlignment.Fill

                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }

                    hintText: ""
                }

                Button {
                    horizontalAlignment: HorizontalAlignment.Center

                    text: qsTr ("Run Script")
                    onClicked: _gameController.run(scriptContent.text)
                }
//! [1]
            }
        }
    }
}
