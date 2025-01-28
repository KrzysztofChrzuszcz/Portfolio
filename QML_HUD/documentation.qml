import QtQuick 2.15

/*!
    \brief Documentation for QML HUD Components
    
    This file contains documentation and examples for all HUD controls.
*/
QtObject {
    /*!
        \qmltype AbstractControl
        \brief Base component providing common properties for all HUD controls
        
        AbstractControl serves as the base component for all HUD controls,
        providing common functionality for scaling, positioning, and coloring.
        
        Properties:
        \list
            \li size (real): Size of the control rectangle (both sides are equal)
            \li scale (real): Scale factor for the control view, centered. Default: 1.0
            \li horizontalshift (real): Horizontal offset of the control view in pixels
            \li verticalshift (real): Vertical offset of the control view in pixels
            \li mirrorview (bool): Enables mirroring of the control view
            \li activecolor (color): Color for the value indicator
            \li nonactivecolor (color): Color for the track
            \li backgroundcolor (color): Color for the control background
            \li backlightcolor (color): Color for the backlight effect
            \li dialcolor (color): Color for the dial markings and text
            \li dialfontsize (int): Font size for dial labels
            \li tracktodialspacing (int): Space between track and dial in pixels
            \li minvalue (real): Minimum possible value
            \li maxvalue (real): Maximum possible value
            \li value (real): Current value to display
        \endlist
        
        Example:
        \code
        AbstractControl {
            size: 200
            scale: 1.0
            horizontalshift: 0
            verticalshift: 0
            activecolor: "darkCyan"
            nonactivecolor: "gray"
            backgroundcolor: "darkCyan"
            backlightcolor: "cyan"
            dialcolor: "black"
            minvalue: 0
            maxvalue: 100
            value: 50
        }
        \endcode
    */
    
    /*!
        \qmltype VerticalBar
        \inherits AbstractControl
        \brief Vertical progress indicator
        
        VerticalBar provides a vertical progress indicator with customizable
        rotation and orientation.
        
        Additional Properties:
        \list
            \li angle (real): Rotation angle of the bar
            \li diallinewidth (int): Width of dial lines
            \li upsidedown (bool): Flips the bar orientation
        \endlist
        
        Example:
        \code
        VerticalBar {
            size: 300
            scale: 1.2
            angle: -30
            diallinewidth: 2
            upsidedown: false
            activecolor: "darkCyan"
            value: 75
            maxvalue: 100
            dialfontsize: 12
            tracktodialspacing: 10
        }
        \endcode
    */
    
    /*!
        \qmltype Gauge
        \inherits AbstractControl
        \brief Circular gauge indicator
        
        Gauge provides a circular progress indicator with customizable
        start angle and span.
        
        Additional Properties:
        \list
            \li boundingtotrackspacing (int): Space between bounding box and track
            \li spanangle (real): Total angle span of the gauge
            \li startangle (real): Starting angle of the gauge
            \li progresswidth (int): Width of the progress track
        \endlist
        
        Example:
        \code
        Gauge {
            size: 250
            scale: 1.0
            spanangle: 270
            startangle: -135
            progresswidth: 8
            activecolor: "darkCyan"
            nonactivecolor: "gray"
            value: 60
            maxvalue: 100
            dialfontsize: 10
            boundingtotrackspacing: 5
        }
        \endcode
    */
    
    /*!
        \qmltype HorizontalBoard
        \inherits AbstractControl
        \brief Horizontal indicator board
        
        HorizontalBoard provides a horizontal progress indicator with
        customizable dial types and diversion modes.
        
        Additional Properties:
        \list
            \li diverted (bool): Enables diverted mode
            \li dialtype (Enum::DialType): Type of dial display ("Full", "Upper", "Bottom")
        \endlist
        
        Example:
        \code
        HorizontalBoard {
            size: 400
            scale: 1.0
            diverted: true
            dialtype: HorizontalBoard.Full
            activecolor: "darkCyan"
            nonactivecolor: "gray"
            value: 80
            maxvalue: 100
            dialfontsize: 12
            tracktodialspacing: 15
        }
        \endcode
    */
}