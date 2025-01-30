//! https://doc.qt.io/qt-5/qtquickcontrols-index.html
import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import CustomControls 1.0
import Themes 1.0

Window {
	width: 926
    height: 428
    visible: true
    title: "QML_HUD"

    ThemeProvider {
        id: lightTheme
        Component.onCompleted: {
            if (!loadFromJson("M:/Repo/Portfolio/QML_HUD/lightTheme.json")) {
                console.error("Failed to load light theme");
            }
        }
    }

    ThemeProvider {
        id: darkTheme
        Component.onCompleted: {
            if (!loadFromJson("M:/Repo/Portfolio/QML_HUD/darkTheme.json")) {
                console.error("Failed to load dark theme");
            }
        }
    }

    property var activeTheme: lightTheme.themeMap
	property real progressValue: backend.value
	property bool mirror: backend.mirror;
	

	Rectangle {
		anchors.fill: parent
		
		Rectangle {
				LayoutMirroring.enabled: mirror
				LayoutMirroring.childrenInherit: mirror
				anchors.fill: parent
				color: activeTheme.backgroundColor || "white"

				Rectangle {
					id: themeSwitchId // nightModeId
					anchors.right: parent.right
					anchors.top: parent.top
					height: 50
					width: height * 2
					color: movingRectangleId.x >= width / 2 ? "gray" : "white"
					border.color: "black"
					border.width: 2
					
					Rectangle {
						id: movingRectangleId
						height: parent.height
						width: parent.width / 2
						border.color: "black"
						border.width: 2
						color: "lightGray"
					}
					MouseArea {
							anchors.fill: movingRectangleId
							drag.target: movingRectangleId
							drag.axis: Drag.XAxis
							drag.minimumX: 0
							drag.maximumX: parent.width - movingRectangleId.width
							
							onPositionChanged: {
								activeTheme = movingRectangleId.x >= themeSwitchId.width / 2 ? darkTheme.themeMap : lightTheme.themeMap;
							}
					}
				}
				
				Row{
					anchors.centerIn: parent
					spacing: 10
				
					Column {
						spacing: 10
						
						VerticalBar {
							id: verticalBar1
							anchors.horizontalCenter: parent.horizontalCenter
							width: 200
							height: 200
							activecolor: activeTheme.progressColor || "darkCyan"
							dialcolor: activeTheme.textColor || "black"
							backgroundcolor: activeTheme.controlBackgroundColor || "darkCyan"
							backlightcolor: activeTheme.controlBacklightColor || "cyan"
							mirrorview: mirror
							sideangle: 10
							value: progressValue
							
							Text {
								id: verticalBar1ValueDisplay
								anchors.horizontalCenter: parent.horizontalCenter
								anchors.verticalCenter: parent.verticalCenter
								font.pixelSize: 20
								font.bold: true
								font.family: activeTheme.valueDisplayFontFamily
								color: activeTheme.textColor || "black"
								text: Math.round(progressValue * 100) + "%"
								transform: [
									Scale {
										xScale: mirror ? -1 : 1
										yScale: 1
									},
									Translate {
										x: mirror ? verticalBar1ValueDisplay.width : 0
									}
								]
							}
						}
						
						Text {
							id: verticalBar1label
							anchors.horizontalCenter: parent.horizontalCenter 
							text: "Turbo Bar"
							font.pixelSize: 18
							font.family: activeTheme.labelFontFamily
							color: activeTheme.textColor || "black"
							transform: [
								Scale {
									xScale: mirror ? -1 : 1
									yScale: 1
								},
								Translate {
									x: mirror ? verticalBar1label.width : 0
								}
							]
						}
					}

					Column {
						anchors.bottom: parent.bottom
						spacing: 10
						
						HorizontalBoard {
							id: board1
							anchors.horizontalCenter: parent.horizontalCenter
							width: 200
							height: 200
							dialtype: HorizontalBoard.Bottom
							activecolor: activeTheme.progressColor || "darkCyan"
							dialcolor: activeTheme.textColor || "black"
							backgroundcolor: activeTheme.controlBackgroundColor || "darkCyan"
							backlightcolor: activeTheme.controlBacklightColor || "cyan"
							minvalue: 0
							maxvalue: 20
							verticalshift: 75
							mirrorview: mirror
							value: progressValue
							
							Text {
								id: board1ValueDisplay
								anchors.horizontalCenter: parent.horizontalCenter
								anchors.verticalCenter: parent.verticalCenter
								anchors.verticalCenterOffset: 40
								font.pixelSize: 20
								font.family: activeTheme.valueDisplayFontFamily
								color: activeTheme.textColor || "black"
								text: Math.round(progressValue * 200)/10 + "l/100km"
								transform: [
									Scale {
										xScale: mirror ? -1 : 1
										yScale: 1
									},
									Translate {
										x: mirror ? board1ValueDisplay.width : 0
									}
								]
							}
						}
						
						Text {
							id: boardlabel
							anchors.horizontalCenter: parent.horizontalCenter 
							text: "Fuel Consumption"
							font.pixelSize: 18
							font.family: activeTheme.labelFontFamily
							color: activeTheme.textColor || "black"
							transform: [
								Scale {
									xScale: mirror ? -1 : 1
									yScale: 1
								},
								Translate {
									x: mirror ? boardlabel.width : 0
								}
							]
						}
						
						Slider {
							width: 200
							value: progressValue
							onValueChanged: {
								console.log(value);
								backend.value = value;
								progressValue = value;
								}
						}
						
						Text {
							id: slider1label
							anchors.horizontalCenter: parent.horizontalCenter 
							text: "Test Input"
							font.pixelSize: 18
							font.family: activeTheme.labelFontFamily
							color: activeTheme.textColor || "black"
							transform: [
								Scale {
									xScale: mirror ? -1 : 1
									yScale: 1
								},
								Translate {
									x: mirror ? slider1label.width : 0
								}
							]
						}
					}
					
					Column {
						spacing: 10
						
						Gauge {
							id: gauge1
							anchors.horizontalCenter: parent.horizontalCenter
							width: 200
							height: 200
							activecolor: activeTheme.progressColor || "darkCyan"
							dialcolor: activeTheme.textColor || "black"
							backgroundcolor: activeTheme.controlBackgroundColor || "darkCyan"
							backlightcolor: activeTheme.controlBacklightColor || "cyan"
							verticalshift: 35
							mirrorview: mirror
							value: progressValue
							
							Text {
								id: gauge1ValueDisplay
								anchors.horizontalCenter: parent.horizontalCenter
								anchors.verticalCenter: parent.verticalCenter
								anchors.verticalCenterOffset: 30
								font.pixelSize: 20
								font.family: activeTheme.valueDisplayFontFamily
								color: activeTheme.textColor || "black"
								text: Math.round(progressValue * 280) + "km/h"
								transform: [
									Scale {
										xScale: mirror ? -1 : 1
										yScale: 1
									},
									Translate {
										x: mirror ? gauge1ValueDisplay.width : 0
									}
								]
							}
						}
						
						Text {
							id: gauge1label
							anchors.horizontalCenter: parent.horizontalCenter 
							text: "Speed Gauge"
							font.family: activeTheme.labelFontFamily
							font.pixelSize: 18
							color: activeTheme.textColor || "black"
							transform: [
								Scale {
									xScale: mirror ? -1 : 1
									yScale: 1
								},
								Translate {
									x: mirror ? gauge1label.width : 0
								}
							]
						}
					}
				}
			}
			
			Button {
					anchors.right: parent.right
					anchors.bottom: parent.bottom
					text: "Mirror"
					onClicked: {
						backend.printSth()
						mirror = !mirror
						backend.mirror = mirror
					}
				}
    }
}
