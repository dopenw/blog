# 《Qml book》 - Quick Starter

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [《Qml book》 - Quick Starter](#qml-book-quick-starter)
  - [QML Syntax](#qml-syntax)
    - [Properties](#properties)
    - [Scripting](#scripting)
  - [Basic Elements](#basic-elements)
    - [Item Element](#item-element)
    - [Rectangle Element](#rectangle-element)
    - [Text Element](#text-element)
    - [Image Element](#image-element)
    - [MouseArea Element](#mousearea-element)
  - [Components](#components)
  - [Simple Transformations](#simple-transformations)
  - [Source code](#source-code)

<!-- /code_chunk_output -->

 注：该文档为 《Qml Book》的学习文档，详见 [Qml Book - Quick Starter](https://qmlbook.github.io/ch04-qmlstart/qmlstart.html#)

## QML Syntax

In its simplest way, QML is a hierarchy of elements. Child elements inherit the coordinate system from the parent. An x,y coordinate is always relative to the parent.

Let’s start with a simple example of a QML file to explain the different syntax.

```qml
// RectangleExample.qml

import QtQuick 2.5

// The root element is the Rectangle
Rectangle {
    // name this element root
    id: root

    // properties: <name>: <value>
    width: 120; height: 240

    // color property
    color: "#4A4A4A"

    // Declare a nested element (child of root)
    Image {
        id: triangle

        // reference the parent
        x: (parent.width - width)/2; y: 40

        source: 'assets/triangle_red.png'
    }

    // Another child of root
    Text {
        // un-named element

        // reference element by id
        y: triangle.y + triangle.height + 20

        // reference root element
        width: root.width

        color: 'white'
        horizontalAlignment: Text.AlignHCenter
        text: 'Triangle'
    }
}
```

- The `import` statement imports a module in a specific version.
- Comments can be made using `//` for single line comments or `/* */` for multi-line comments. Just like in C/C++ and JavaScript
- Every QML file needs to have exactly one root element, like HTML
- An element is declared by its type followed by `{ }`
- Elements can have properties, they are in the form `name: value`
- Arbitrary elements inside a QML document can be accessed by using their id (an unquoted identifier)
- Elements can be nested, meaning a parent element can have child elements. The parent element can be accessed using the parent keyword

![](../images/qmlBook_4_quickStarter_202103252257_1.png)

### Properties

Elements are declared by using their element name but are defined by using their properties or by creating custom properties. A property is a simple key-value pair, e.g. `width : 100, text: 'Greetings', color: '#FF0000'`. A property has a well-defined type and can have an initial value.

```qml
 Text {
        // (1) identifier
        id: thisLabel

        // (2) set x- and y-position
        x: 24; y: 16

        // (3) bind height to 2 * width
        height: 2 * width

        // (4) custom property
        property int times: 24

        // (5) property alias
        property alias anotherTimes: thisLabel.times

        // (6) set text appended by value
        text: "Greetings " + times

        // (7) font is a grouped property
        font.family: "Ubuntu"
        font.pixelSize: 24

        // (8) KeyNavigation is an attached property
        KeyNavigation.tab: otherLabel

        // (9) signal handler for property changes
        onHeightChanged: console.log('height:', height)

        // focus is need to receive key events
        focus: true

        // change color based on focus value
        color: focus?"red":"black"
    }
```

Let’s go through the different features of properties:

- `id` is a very special property-like value, it is used to reference elements inside a QML file (called “document” in QML). The `id` is not a string type but rather an identifier and part of the QML syntax. An id needs to be unique inside a document and it can’t be reset to a different value, nor may it be queried. (It behaves much like a reference in the C++ world.)
- A property can be set to a value, depending on its type. If no value is given for a property, an initial value will be chosen. You need to consult the documentation of the particular element for more information about the initial value of a property.
- A property can depend on one or many other properties. This is called binding. A bound property is updated when its dependent properties change. It works like a contract, in this case, the `height` should always be two times the `width`.
- Adding own properties to an element is done using the `property` qualifier followed by the type, the name and the optional initial value (`property <type> <name> : <value>`). If no initial value is given a system initial value is chosen.
- Another important way of declaring properties is using the `alias` keyword (`property alias <name>: <reference>`). The `alias` keyword allows us to forward a property of an object or an object itself from within the type to an outer scope. We will use this technique later when defining components to export the inner properties or element ids to the root level. A property alias does not need a type, it uses the type of the referenced property or object.
- The text property depends on the custom property `times` of type int. The `int` based value is automatically converted to a `string` type. The expression itself is another example of binding and results in the text being updated every time the `times` property changes.
- Some properties are grouped properties. This feature is used when a property is more structured and related properties should be grouped together. Another way of writing grouped properties is `font { family: "Ubuntu"; pixelSize: 24 }`.
- Some properties are attached to the element itself. This is done for global relevant elements which appear only once in the application (e.g. keyboard input). The writing is`<Element>.<property>: <value>`.
- For every property, you can provide a signal handler. This handler is called after the property changes. For example, here we want to be notified whenever the height changes and use the built-in console to log a message to the system.

### Scripting

QML and JavaScript are best friends. In the JavaScript chapter we will go into more detail on this symbiosis. Currently, we just want to make you aware of this relationship.

```qml
Text {
        id: label

        x: 24; y: 24

        // custom counter property for space presses
        property int spacePresses: 0

        text: "Space pressed: " + spacePresses + " times"

        // (1) handler for text changes
        onTextChanged: console.log("text changed to:", text)

        // need focus to receive key events
        focus: true

        // (2) handler with some JS
        Keys.onSpacePressed: {
            increment()
        }

        // clear the text on escape
        Keys.onEscapePressed: {
            label.text = ''
        }

        // (3) a JS function
        function increment() {
            spacePresses = spacePresses + 1
        }
    }
```

1. The text changed handler `onTextChanged` prints the current text every-time the text changed due to a space-bar key pressed
2. When the text element receives the space-bar key (because the user pressed the space-bar on the keyboard) we call a JavaScript function `increment()`.
3. Definition of a JavaScript function in the form of `function <name>(<parameters>) { ... }`, which increments our counter `spacePressed`. Every time `spacePressed` is incremented, bound properties will also be updated.

Note:
The difference between the QML `:` (binding) and the JavaScript `=` (assignment) is, that the binding is a contract and keeps true over the lifetime of the binding, whereas the JavaScript assignment (`=`) is a one time value assignment. The lifetime of a binding ends, when a new binding is set to the property or even when a JavaScript value is assigned is to the property.

## Basic Elements

Elements can be grouped into visual and non-visual elements:

- A visual element (like the `Rectangle`) has a geometry and normally presents an area on the screen.
- A non-visual element (like a `Timer`) provides general functionality, normally used to manipulate the visual elements.

Currently, we will focus on the fundamental visual elements, such as `Item`, `Rectangle`, `Text`, `Image` and `MouseArea`. However, by using the Qt Quick Controls 2 module, it is possible to create user interfaces built from standard platform components such as buttons, labels and sliders.

### Item Element

`Item` is the base element for all visual elements as such all other visual elements inherits from `Item`. It doesn’t paint anything by itself but defines all properties which are common across all visual elements:
|       Group       |                                                                                    Properties                                                                                     |
|:----------------: |:--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: |
|     Geometry      | `x` and `y` to define the top-left position, `width` and `height` for the expansion of the element and also the `z` stacking order to lift elements up or down from their natural ordering  |
|  Layout handling  |                       `anchors` (left, right, top, bottom, vertical and horizontal center) to position elements relative to other elements with their `margins`                       |
|   Key handling    |                     attached `Key` and `KeyNavigation` properties to control key handling and the input `focus` property to enable key handling in the first place                      |
|  Transformation   |                         `scale` and `rotate` transformation and the generic `transform` property list for x,y,z transformation and their `transformOrigin` point                          |
|      Visual       |       `opacity` to control transparency, `visible` to show/hide elements, `clip` to restrain paint operations to the element boundary and `smooth` to enhance the rendering quality       |
| State definition  |               `states` list property with the supported list of states and the current `state` property as also the `transitions` list property to animate state changes.               |

Note

The `Item` element is often used as a container for other elements, similar to the div element in HTML.

### Rectangle Element

The `Rectangle` extends `Item` and adds a fill color to it. Additionally it supports borders defined by `border.color` and `border.width`. To create rounded rectangles you can use the `radius` property.

```qml
    Rectangle {
        id: rect1
        x: 12; y: 12
        width: 76; height: 96
        color: "lightsteelblue"
    }
    Rectangle {
        id: rect2
        x: 112; y: 12
        width: 76; height: 96
        border.color: "lightsteelblue"
        border.width: 4
        radius: 8
    }
```

![](../images/qmlBook_4_quickStarter_202103252257_2.png)

Besides a fill color and a border the rectangle also supports custom gradients.

```qml
    Rectangle {
        id: rect1
        x: 12; y: 12
        width: 176; height: 96
        gradient: Gradient {
            GradientStop { position: 0.0; color: "lightsteelblue" }
            GradientStop { position: 1.0; color: "slategray" }
        }
        border.color: "slategray"
    }
```

![](../images/qmlBook_4_quickStarter_202103252257_3.png)

A gradient is defined by a series of gradient stops. Each stop has a position and a color. The position marks the position on the y-axis (0 = top, 1 = bottom). The color of the `GradientStop` marks the color at that position.

Note:

- A rectangle with no width/height set will not be visible. This happens often when you have several rectangles width (height) depending on each other and something went wrong in your composition logic. So watch out!
- It is not possible to create an angled gradient. For this, it’s better to use predefined images. One possibility would be to just rotate the rectangle with the gradient, but be aware the geometry of a rotated rectangle will not change and thus will lead to confusion as the geometry of the element is not the same as the visible area. From the authors perspective, it’s really better to use designed gradient images in that case.

### Text Element

To display text, you can use the `Text` element. Its most notable property is the text property of type `string`. The element calculates its initial width and height based on the given text and the font used. The font can be influenced using the font property group (e.g. `font.family`, `font.pixelSize`, …). To change the color of the text just use the color property.

```qml
    Text {
        text: "The quick brown fox"
        color: "#303030"
        font.family: "Ubuntu"
        font.pixelSize: 28
    }
```

![](../images/qmlBook_4_quickStarter_202103252257_4.png)

Text can be aligned to each side and the center using the `horizontalAlignment` and `verticalAlignment` properties.
To further enhance the text rendering you can use the `style` and `styleColor` property, which allows you render the text in outline, raised and sunken mode.
For longer text, you often want to define a break position like A very … long text, this can be achieved using the `elide` property. The `elide` property allows you to set the elide position to the left, right or middle of your text. In case you don’t want the ‘…’ of the elide mode to appear but still want to see the full text you can also wrap the text using the `wrapMode` property (works only when the width is explicitly set):

```qml
Text {
    width: 40; height: 120
    text: 'A very long text'
    // '...' shall appear in the middle
    elide: Text.ElideMiddle
    // red sunken text styling
    style: Text.Sunken
    styleColor: '#FF4444'
    // align text to the top
    verticalAlignment: Text.AlignTop
    // only sensible when no elide mode
    // wrapMode: Text.WordWrap
}
```

Note

- Be aware a `Text` initial width (height) is depending on the text string and on the font set. A `Text` element with no width set and no text will not be visible, as the initial width will be 0.
- Often when you want to layout `Text` elements you need to differentiate between aligning the text inside the `Text` element boundary box or to align the element boundary box itself. In the former, you want to use the `horizontalAlignment` and `verticalAlignment` properties and in the latter case, you want to manipulate the element geometry or use anchors.

### Image Element

An `Image` element is able to display images in various formats (e.g. PNG, JPG, GIF, BMP, WEBP). For the full list of supported image formats, please consult the Qt documentation. Besides the obvious `source` property to provide the image URL, it contains a `fillMode` which controls the resizing behavior.

```qml
    Image {
        x: 12; y: 12
        // width: 72
        // height: 72
        source: "assets/triangle_red.png"
    }
    Image {
        x: 12+64+12; y: 12
        // width: 72
        height: 72/2
        source: "assets/triangle_red.png"
        fillMode: Image.PreserveAspectCrop
        clip: true
    }
```

![](../images/qmlBook_4_quickStarter_202103252257_5.png)

Note:

- A URL can be a local path with forward slashes ( “./images/home.png” ) or a web-link (e.g. “<http://example.org/home.png>”).
- `Image` elements using `PreserveAspectCrop` should also enable the clipping to avoid image data being rendered outside the `Image` boundaries. By default clipping is disabled (``clip: false``). You need to enable clipping (``clip: true``) to constrain the painting to the elements bounding rectangle. This can be used on any visual element.

Tip:

- Using C++ you are able to create your own image provider using [QQuickImageProvider](https://doc.qt.io/qt-5//qquickimageprovider.html). This allows you to create images on the fly and threaded image loading.

### MouseArea Element

To interact with these elements you often will use a `MouseArea`. It’s a rectangular invisible item in where you can capture mouse events. The mouse area is often used together with a visible item to execute commands when the user interacts with the visual part.

```qml
    Rectangle {
        id: rect1
        x: 12; y: 12
        width: 76; height: 96
        color: "lightsteelblue"
        MouseArea {
            id: area
            width: parent.width
            height: parent.height
            onClicked: rect2.visible = !rect2.visible
        }
    }

    Rectangle {
        id: rect2
        x: 112; y: 12
        width: 76; height: 96
        border.color: "lightsteelblue"
        border.width: 4
        radius: 8
    }
```

![](../images/qmlBook_4_quickStarter_202103252257_6.png)

Note:

- For more complex interaction, [Qt Quick Input Handlers](https://doc.qt.io/qt-5/qtquickhandlers-index.html) where introduced with Qt 5.12. They are intended to be used instead of elements such as `MouseArea` and `Flickable` and offer greater control and flexibility. The idea is to handle one interaction aspect in each handler instance instead of centralizing the handling of all events from a given source in a single element, which was the case before.

## Components
A component is a reusable element and QML provides different ways to create components. Currently, we will look only at the simplest form - a file-based component. A file-based component is created by placing a QML element in a file and give the file an element name (e.g. `Button.qml`). You can use the component like every other element from the QtQuick module, in our case you would use this in your code as `Button` { ... }.

For example, let’s create a rectangle containing a text component and a mouse area. This resembles a simple button and doesn’t need to be more complicated for our purposes.

```qml
    Rectangle { // our inlined button ui
        id: button
        x: 12; y: 12
        width: 116; height: 26
        color: "lightsteelblue"
        border.color: "slategrey"
        Text {
            anchors.centerIn: parent
            text: "Start"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                status.text = "Button clicked!"
            }
        }
    }

    Text { // text changes when button was clicked
        id: status
        x: 12; y: 76
        width: 116; height: 26
        text: "waiting ..."
        horizontalAlignment: Text.AlignHCenter
    }
```

![](../images/qmlBook_4_quickStarter_202103262213_1.png)

Our task is now to extract the button UI in a reusable component. For this, we shortly think about a possible API for our button. You can do this by imagining how someone else should use your button. Here’s what I came up with:
```qml
// minimal API for a button
Button {
    text: "Click Me"
    onClicked: { /* do something */ }
}
```

I would like to set the `text` using a text property and to implement my own click handler. Also, I would expect the button to have a sensible initial size, which I can overwrite (e.g. with `width: 240` for example).

To achieve this we create a `Button.qml` file and copy our button UI inside. Additionally, we need to export the properties a user might want to change on the root level.

```qml
// Button.qml

import QtQuick 2.5

Rectangle {
    id: root
    // export button properties
    property alias text: label.text
    signal clicked

    width: 116; height: 26
    color: "lightsteelblue"
    border.color: "slategrey"

    Text {
        id: label
        anchors.centerIn: parent
        text: "Start"
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            root.clicked()
        }
    }
}

```

We have exported the text and clicked signal on the root level. Typically we name our root element root to make the referencing easier. We use the `alias` feature of QML, which is a way to export properties inside nested QML elements to the root level and make this available for the outside world. It is important to know, that only the root level properties can be accessed from outside this file by other components.

To use our new `Button` element we can simply declare it in our file. So the earlier example will become a little bit simplified.
```qml
    Button { // our Button component
        id: button
        x: 12; y: 12
        text: "Start"
        onClicked: {
            status.text = "Button clicked!"
        }
    }

    Text { // text changes when button was clicked
        id: status
        x: 12; y: 76
        width: 116; height: 26
        text: "waiting ..."
        horizontalAlignment: Text.AlignHCenter
    }
```

Note:
- If you want to, you could even go a step further and use an item as a root element. This prevents users to change the color of our designed button and provides us with more control about the exported API. The target should be to export a minimal API. Practically this means we would need to replace the root `Rectangle` with an `Item` and make the rectangle a nested element in the root item.

```qml
Item {
    id: root
    width: 116; height: 26

    property alias text: label.text
    signal clicked

    Rectangle {
        anchors.fill parent
        color: "lightsteelblue"
        border.color: "slategrey"
    }
    ...
}
```

## Simple Transformations

A transformation manipulates the geometry of an object. QML Items can, in general, be translated, rotated and scaled. There is a simple form of these operations and a more advanced way.

Let’s start with the simple transformations. Here is our scene as our starting point.

A simple translation is done via changing the `x,y` position. A `rotation` is done using the rotation property. The value is provided in degrees (0 .. 360). A scaling is done using the `scale` property and a value <1 means the element is scaled down and `>1` means the element is scaled up. The rotation and scaling do not change your geometry. The items `x,y` and `width/height` haven’t changed. Just the painting instructions are transformed.

Before we show off the example I would like to introduce a little helper: The `ClickableImage` element. 

```qml
// ClickableImage.qml

// Simple image which can be clicked

import QtQuick 2.5

Image {
    id: root
    signal clicked

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked()
    }
}
```

![](../images/qmlBook_4_quickStarter_202103262213_2.png)

We use our clickable image to present three objects (box, circle, triangle). Each object performs a simple transformation when clicked. Clicking the background will reset the scene.

```qml
// transformation.qml


import QtQuick 2.5

Item {
    // set width based on given background
    width: bg.width
    height: bg.height

    Image { // nice background image
        id: bg
        source: "assets/background.png"
    }

    MouseArea {
        id: backgroundClicker
        // needs to be before the images as order matters
        // otherwise this mousearea would be before the other elements
        // and consume the mouse events
        anchors.fill: parent
        onClicked: {
            // reset our little scene
            circle.x = 84
            box.rotation = 0
            triangle.rotation = 0
            triangle.scale = 1.0
        }
    }

    ClickableImage {
        id: circle
        x: 84; y: 68
        source: "assets/circle_blue.png"
        antialiasing: true
        onClicked: {
            // increase the x-position on click
            x += 20
        }
    }

    ClickableImage {
        id: box
        x: 164; y: 68
        source: "assets/box_green.png"
        antialiasing: true
        onClicked: {
            // increase the rotation on click
            rotation += 15
        }
    }

    ClickableImage {
        id: triangle
        x: 248; y: 68
        source: "assets/triangle_red.png"
        antialiasing: true
        onClicked: {
            // several transformations
            rotation += 15
            scale += 0.05
        }
    }

    function _test_transformed() {
        circle.x += 20
        box.rotation = 15
        triangle.scale = 1.2
        triangle.rotation = -15
    }

    function _test_overlap() {
        circle.x += 40
        box.rotation = 15
        triangle.scale = 2.0
        triangle.rotation = 45
    }

}
```

![](../images/qmlBook_4_quickStarter_202103262213_3.png)

The circle increments the x-position on each click and the box will rotate on each click. The triangle will rotate and scale the image up on each click, to demonstrate a combined transformation. For the scaling and rotation operation we set `antialiasing: true` to enable anti-aliasing, which is switched off (same as the clipping property `clip`) for performance reasons. In your own work, when you see some rasterized edges in your graphics, then you should probably switch smoothly on.

Note:
- To achieve better visual quality when scaling images it is recommended to scale images down instead of up. Scaling an image up with a larger scaling factor will result in scaling artifacts (blurred image). When scaling an image you should consider using `` antialiasing: true`` to enable the usage of a higher quality filter.
- Elements which appear earlier in the code have a lower stacking order (called z-order). If you click long enough on `circle` you will see it moves below `box`. The z-order can also be manipulated by the `z-property` of an Item.

![](../images/qmlBook_4_quickStarter_202103262213_4.png)

## Source code

[source code](https://qmlbook.github.io/assets/ch04-qmlstart-assets.tgz)

- [上一级](README.md)
- 上一篇 -> [《Qml Book》 - Meet Qt5](qmlBook_1_MeetQt5.md)
- 下一篇 -> [qwt 简单使用](qwt.md)
