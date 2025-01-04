//! https://doc.qt.io/qt-5/qtquickcontrols-index.html
import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.2
import CustomControls 1.0 // Private own controls


Window {
	property real val: 0.0;
	property color textColor: "black";
	property color progressColor: "darkCyan";
	property bool mirror: false;
	

    width: 926
    height: 428
    visible: true
    title: "Hello World"
	
	Rectangle {
		anchors.fill: parent
		
		Rectangle {
				LayoutMirroring.enabled: mirror
				LayoutMirroring.childrenInherit: mirror
				anchors.fill: parent
				color: movingRectangleId.x >= themeId.width / 2 ? "gray" : "white"

				Rectangle {
					id: themeId // nightModeId
					anchors.right: parent.right
					anchors.top: parent.top
					height: 50
					width: height * 2
					color: movingRectangleId.x >= width / 2 ? "gray" : "white"
					// TODO: to CSS
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
								textColor = movingRectangleId.x >= themeId.width / 2 ? "white" : "black"
								progressColor = movingRectangleId.x >= themeId.width / 2 ? "cyan" : "darkCyan"
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
							activecolor: progressColor
							value: val
						}
						
						Text {
							id: verticalBar1label
							anchors.horizontalCenter: parent.horizontalCenter 
							text: "Turbo Bar"
							font.pixelSize: 18
							color: textColor
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
						
						Slider {
							width: 200
							onValueChanged: {
								console.log(value)
								val = value;
								}
						}
						
						Text {
							id: slider1label
							anchors.horizontalCenter: parent.horizontalCenter 
							text: "Test Input"
							font.pixelSize: 18
							color: textColor
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
							activecolor: progressColor
							value: val
						}
						
						Text {
							id: gauge1label
							anchors.horizontalCenter: parent.horizontalCenter 
							text: "Speed Gauge"
							font.pixelSize: 18
							color: textColor
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
					}
				}
    }
}
