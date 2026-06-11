import QtQuick
import QtQuick.Controls
import QtQuick.Window
import FractalApp 1.0

ApplicationWindow {
    id: mainWin
    title: "Fractal Julia 4D - Render Engine"
    visibility: "FullScreen"
    color: "#1e272e"

    Row {
        anchors.fill: parent

        Rectangle {
            width: 350
            color: "#2f3640"
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            Column {
                anchors.fill: parent
                anchors.margins: 25
                spacing: 20

                Text {
                    text: "Ustawienia Rendera"
                    font.pointSize: 18
                    font.bold: true
                    color: "#f5f6fa"
                }

                // 1. CPU / GPU
                ComboBox {
                    id: deviceBox
                    width: parent.width
                    model: ["GPU (CUDA)", "CPU (Seryjnie)"]
                    onCurrentIndexChanged: {
                        if (fractalRender.isRunning)
                            fractalRender.isRunning = false
                    }
                }

                // 2. WYMIAR
                Column {
                    width: parent.width
                    spacing: 6

                    Text {
                        text: "Wymiar fraktala:"
                        color: "#f5f6fa"
                        font.pointSize: 11
                    }

                    ComboBox {
                        id: dimensionBox
                        width: parent.width
                        model: ["3D", "4D"]
                        currentIndex: 0
                        onCurrentIndexChanged: {
                            fractalRender.isRunning = false
                            fractalRender.dimension = currentIndex + 3
                        }
                    }
                }

                // 3a. PARAMETRY C (tylko 3D)
                Column {
                    width: parent.width
                    spacing: 8
                    visible: dimensionBox.currentIndex === 0

                    Text {
                        text: "Parametry C fraktala:"
                        color: "#f5f6fa"
                        font.pointSize: 11
                        font.bold: true
                    }

                    Repeater {
                        model: ["C.x", "C.y", "C.z", "C.w"]

                        Row {
                            width: parent.width
                            spacing: 8

                            Text {
                                text: modelData + ":"
                                color: "#7f8fa6"
                                font.pointSize: 10
                                width: 35
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            Rectangle {
                                width: parent.width - 43
                                height: 32
                                color: "#1e272e"
                                radius: 6
                                border.color: cField.activeFocus ? "#4cd137" : "#576574"
                                border.width: 1

                                TextInput {
                                    id: cField
                                    anchors.fill: parent
                                    anchors.margins: 8
                                    color: "#f5f6fa"
                                    font.pointSize: 10
                                    text: index === 0 ? "-0.2" :
                                            index === 1 ? "0.8"  :
                                                index === 2 ? "0.0"  : "0.0"
                                    validator: DoubleValidator { bottom: -10.0; top: 10.0; decimals: 4 }
                                    selectByMouse: true
                                    onEditingFinished: {
                                        var v = parseFloat(text)
                                        if (isNaN(v)) return
                                        if (index === 0) fractalRender.setCx(v)
                                        if (index === 1) fractalRender.setCy(v)
                                        if (index === 2) fractalRender.setCz(v)
                                        if (index === 3) fractalRender.setCw(v)
                                        focusItem.forceActiveFocus()
                                    }
                                }
                            }
                        }
                    }
                }

                // 3b. OŚ ANIMACJI (tylko 4D)
                Column {
                    width: parent.width
                    spacing: 6
                    visible: dimensionBox.currentIndex === 1

                    Text {
                        text: "Oś animacji 4D:"
                        color: "#f5f6fa"
                        font.pointSize: 11
                    }

                    ComboBox {
                        id: axisBox
                        width: parent.width
                        model: ["C.x", "C.y", "C.z", "C.w"]
                        currentIndex: 2
                        onCurrentIndexChanged: {
                            fractalRender.animAxis = currentIndex
                        }
                    }
                }

                // 4. ROZDZIELCZOŚĆ
                Column {
                    width: parent.width
                    spacing: 6

                    Text {
                        text: "Rozdzielczość: " + Math.round(focusItem.width * scaleSlider.value) + " x " + Math.round(focusItem.height * scaleSlider.value)
                        color: "#f5f6fa"
                        font.pointSize: 11
                    }

                    Slider {
                        id: scaleSlider
                        width: parent.width
                        from: 0.333
                        to: 1.0
                        value: 1.0
                        stepSize: 0.01
                        enabled: !(deviceBox.currentIndex === 1 && fractalRender.isRunning)
                        opacity: enabled ? 1.0 : 0.4
                        onValueChanged: fractalRender.renderScale = value
                    }

                    Text {
                        visible: deviceBox.currentIndex === 1 && fractalRender.isRunning
                        text: "Zatrzymaj render aby zmienić"
                        color: "#e17055"
                        font.pointSize: 9
                    }
                }

                // 5. URUCHOM RENDER
                Button {
                    width: parent.width
                    height: 50
                    text: fractalRender.isRunning ? "ZATRZYMAJ RENDER" : "URUCHOM RENDER"
                    font.bold: true
                    onClicked: {
                        if (fractalRender.isRunning) {
                            fractalRender.isRunning = false
                        } else {
                            fractalRender.useGPU = (deviceBox.currentIndex === 0)
                            fractalRender.resetBuffers()
                            fractalRender.isRunning = true
                            focusItem.forceActiveFocus()
                        }
                    }
                }

                // 6. CZAS RENDERU
                Rectangle {
                    width: parent.width
                    height: 75
                    color: "#353b48"
                    radius: 8
                    Column {
                        anchors.centerIn: parent
                        spacing: 5
                        Text { text: "Czas pierwszej klatki:"; color: "#7f8fa6" }
                        Text { text: fractalRender.renderTime; color: "#4cd137"; font.bold: true; font.pointSize: 15 }
                    }
                }

                // 7. WYJŚCIE – przykleja się do dołu
                Item {
                    width: parent.width
                    height: parent.height - y - 70
                }

                Button {
                    width: parent.width
                    height: 45
                    text: "WYJŚCIE"
                    font.bold: true
                    onClicked: Qt.quit()
                }
            }
        }

        Item {
            id: focusItem
            width: parent.width - 350
            height: parent.height
            focus: true

            FractalItem {
                id: fractalRender
                anchors.fill: parent
                useGPU: deviceBox.currentIndex === 0
                animAxis: axisBox.currentIndex
            }

            Keys.onPressed: (event) => {
                if (event.key === Qt.Key_W) fractalRender.setKeyState("W", true)
                if (event.key === Qt.Key_S) fractalRender.setKeyState("S", true)
                if (event.key === Qt.Key_A) fractalRender.setKeyState("A", true)
                if (event.key === Qt.Key_D) fractalRender.setKeyState("D", true)
            }
            Keys.onReleased: (event) => {
                if (event.key === Qt.Key_W) fractalRender.setKeyState("W", false)
                if (event.key === Qt.Key_S) fractalRender.setKeyState("S", false)
                if (event.key === Qt.Key_A) fractalRender.setKeyState("A", false)
                if (event.key === Qt.Key_D) fractalRender.setKeyState("D", false)
            }

            MouseArea {
                anchors.fill: parent
                property real lastX: 0
                property real lastY: 0
                onPressed: (mouse) => {
                    lastX = mouse.x
                    lastY = mouse.y
                    focusItem.forceActiveFocus()
                }
                onPositionChanged: (mouse) => {
                    if (pressed && fractalRender.isRunning) {
                        fractalRender.rotateCamera(mouse.x - lastX, mouse.y - lastY)
                        lastX = mouse.x
                        lastY = mouse.y
                    }
                }
            }
        }
    }
}