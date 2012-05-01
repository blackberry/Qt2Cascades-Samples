import bb.cascades 1.0

Page {
    content: Container {
        layout: StackLayout {
        }
        Container {
            layout: StackLayout {
                layoutDirection: LayoutDirection.LeftToRight
            }
            topMargin: 10
            leftMargin: 10
            rightMargin: 10
            TextField {
                id: url
                hintText: "Enter URL to download"
                layoutProperties: StackLayoutProperties {
                }
                onTextChanging: {
                    downloadButton.enabled = (text != "")
                }
            }
            Button {
                id: downloadButton
                text: "Download"
                onClicked: _manager.downloadUrl (url.text)
                layoutProperties: StackLayoutProperties {
                }
                leftMargin: 10
                rightMargin: 10
            }
        }
        Label {
            text: "Active Downloads: " + (_manager.activeDownloads == 0 ? "none" : _manager.activeDownloads)
            topMargin: 10
            leftMargin: 10
        }
        Label {
            text: "Status:"
            topMargin: 10
            leftMargin: 10
        }
        TextArea {
            textColor: Color.Green
            backgroundVisible: false
            editable: false
            text: _manager.statusMessage
            preferredWidth: 900
            preferredHeight: 200
            leftMargin: 10
        }
        Label {
            text: "Errors:"
            topMargin: 10
            leftMargin: 10
        }
        TextArea {
            textColor: Color.Red
            backgroundVisible: false
            editable: false
            text: _manager.errorMessage
            preferredWidth: 900
            preferredHeight: 200
            leftMargin: 10
        }
    }
}