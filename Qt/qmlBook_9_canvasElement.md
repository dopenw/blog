# 《Qml Book》 - Canvas Element

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [《Qml Book》 - Canvas Element](#qml-book-canvas-element)
  - [Convenient API](#convenient-api)
  - [Gradients](#gradients)
  - [Shadows](#shadows)
  - [Images](#images)
  - [Transformation](#transformation)
  - [Source code](#source-code)

<!-- /code_chunk_output -->

注：该文档为 《Qml Book》的学习文档，详见 [Qml book - Qt Canvas Element](https://qmlbook.github.io/ch09-canvas/canvas.html)

Early on when QML was introduced in Qt4 there were some discussions about if Qt Quick needs an ellipse. The problem with the ellipse is that others can argue other shapes need also be supported. So there is no ellipse in Qt Quick only rectangular shapes. If you needed one in Qt4 you would need to use an image or write your own C++ ellipse element.

To allow scripted drawings Qt 5 introduces the canvas element. The [canvas](https://doc.qt.io/qt-5/qml-qtquick-canvas.html) elements provide a resolution-dependent bitmap canvas, which can be used for graphics, games or to paint other visual images on the fly using JavaScript. The canvas element is based on the HTML5 canvas element.

The fundamental idea of the canvas element is to render paths using a context 2D object. The context 2D object, contains the necessary graphics functions, whereas the canvas acts as the drawing canvas. The 2D context supports strokes, fills gradients, text and a different set of path creation commands.

Let’s see an example of a simple path drawing:

```qml
import QtQuick 2.5

Canvas {
    id: root
    // canvas size
    width: 200; height: 200
    // handler to override for drawing
    onPaint: {
        // get context to draw with
        var ctx = getContext("2d")
        // setup the stroke
        ctx.lineWidth = 4
        ctx.strokeStyle = "blue"
        // setup the fill
        ctx.fillStyle = "steelblue"
        // begin a new path to draw
        ctx.beginPath()
        // top-left start point
        ctx.moveTo(50,50)
        // upper line
        ctx.lineTo(150,50)
        // right line
        ctx.lineTo(150,150)
        // bottom line
        ctx.lineTo(50,150)
        // left line through path closing
        ctx.closePath()
        // fill using fill style
        ctx.fill()
        // stroke using line width and stroke style
        ctx.stroke()
    }
}
```

This produces a filled rectangle with a starting point at 50,50 and a size of 100 and a stroke used as a border decoration.

![](../images/qmlBook_9_canvasElement_202104192032_1.png)

The stroke width is set to 4 and uses a blue color define by `strokeStyle`. The final shape is set up to be filled through the `fillStyle` to a “steel blue” color. Only by calling `stroke` or `fill` the actual path will be drawn and they can be used independently from each other. A call to `stroke` or `fill` will draw the current path. It’s not possible to store a path for later reuse only a drawing state can be stored and restored.

In QML the `Canvas` element acts as a container for the drawing. The 2D context object provides the actual drawing operation. The actual drawing needs to be done inside the `onPaint` event handler.

```qml
Canvas {
    width: 200; height: 200
    onPaint: {
        var ctx = getContext("2d")
        // setup your path
        // fill or/and stroke
    }
}
```

The canvas itself provides a typical two-dimensional Cartesian coordinate system, where the top-left is the (0,0) point. A higher y-value goes down and a hight x-value goes to the right.

A typical order of commands for this path based API is the following:

1. Setup stroke and/or fill
2. Create path
3. Stroke and/or fill

```qml
 onPaint: {
        var ctx = getContext("2d")

        // setup the stroke
        ctx.strokeStyle = "red"

        // create a path
        ctx.beginPath()
        ctx.moveTo(50,50)
        ctx.lineTo(150,50)

        // stroke path
        ctx.stroke()
    }
```

![](../images/qmlBook_9_canvasElement_202104192032_2.png)

Note

- Typically you always want to set a start point when you reset your path, so the first operation after beginPath is often moveTo.

## Convenient API

For operations on rectangles, a convenience API is provided which draws directly and does need a stroke or fill call.

```qml
// convenient.qml

import QtQuick 2.5

Canvas {
    id: root
    width: 120; height: 120
    onPaint: {
        var ctx = getContext("2d")
        ctx.fillStyle = 'green'
        ctx.strokeStyle = "blue"
        ctx.lineWidth = 4

        // draw a filles rectangle
        ctx.fillRect(20, 20, 80, 80)
        // cut our an inner rectangle
        ctx.clearRect(30,30, 60, 60)
        // stroke a border from top-left to
        // inner center of the larger rectangle
        ctx.strokeRect(20,20, 40, 40)
    }
}
```

![](../images/qmlBook_9_canvasElement_202104192032_3.png)

Note

- The stroke area extends half of the line width on both sides of the path. A 4 px lineWidth will draw 2 px outside the path and 2 px inside.

## Gradients

Canvas can fill shapes with color but also with gradients or images.

```qml
onPaint: {
        var ctx = getContext("2d")

        var gradient = ctx.createLinearGradient(100,0,100,200)
        gradient.addColorStop(0, "blue")
        gradient.addColorStop(0.5, "lightsteelblue")
        ctx.fillStyle = gradient
        ctx.fillRect(50,50,100,100)
    }
```

The gradient in this example is defined along the starting point (100,0) to the end point (100,200), which gives a vertical line in the middle of our canvas. The gradient-stops can be defined as a color from 0.0 (gradient start point) to 1.0 (gradient endpoint). Here we use a “blue” color at 0.0 (100,0) and a “lightsteelblue” color at the 0.5 (100,200) position. The gradient is defined as much larger than the rectangle we want to draw, so the rectangle clips gradient to it’s defined the geometry.

![](../images/qmlBook_9_canvasElement_202104192032_4.png)

Note

- The gradient is defined in canvas coordinates not in coordinates relative to the path to be painted. A canvas does not have the concept of relative coordinates, as we are used to by now from QML.

## Shadows

A path can be visually enhanced using shadows with the 2D context object. A shadow is an area around the path with an offset, color and specified blurring. For this you can specify a `shadowColor`, `shadowOffsetX`, `shadowOffsetY` and a `shadowBlur`. All of this needs to be defined using the 2D context. The 2D context is your only API to the drawing operations.

A shadow can also be used to create a glow effect around a path. In the next example, we create a text “Canvas” with a white glow around. All this on a dark background for better visibility.

First, we draw the dark background:

```qml
        // setup a dark background
        ctx.strokeStyle = "#333"
        ctx.fillRect(0,0,canvas.width,canvas.height);
```

then we define our shadow configuration, which will be used for the next path:

```qml
        // setup a blue shadow
        ctx.shadowColor = "#2ed5fa";
        ctx.shadowOffsetX = 2;
        ctx.shadowOffsetY = 2;
        ctx.shadowBlur = 10;
```

Finally, we draw our “Canvas” text using a large bold 80px font from the Ubuntu font family.

```qml
        // render green text
        ctx.font = 'bold 80px Ubuntu';
        ctx.fillStyle = "#24d12e";
        ctx.fillText("Canvas!",30,180);
```

![](../images/qmlBook_9_canvasElement_202104192032_5.png)

## Images

The QML canvas supports image drawing from several sources. To use an image inside the canvas the image needs to be loaded first. We will use the `Component.onCompleted` handler to load the image in our example.

```qml
    onPaint: {
        var ctx = getContext("2d")


        // draw an image
        ctx.drawImage('assets/ball.png', 10, 10)

        // store current context setup
        ctx.save()
        ctx.strokeStyle = '#ff2a68'
        // create a triangle as clip region
        ctx.beginPath()
        ctx.moveTo(110,10)
        ctx.lineTo(155,10)
        ctx.lineTo(135,55)
        ctx.closePath()
        // translate coordinate system
        ctx.clip()  // create clip from the path
        // draw image with clip applied
        ctx.drawImage('assets/ball.png', 100, 10)
        // draw stroke around path
        ctx.stroke()
        // restore previous context
        ctx.restore()

    }

    Component.onCompleted: {
        loadImage("assets/ball.png")
    }
```

The left shows our ball image painted at the top-left position of `10x10`. The right image shows the ball with a clipping path applied. Images and any other path can be clipped using another path. The clipping is applied by defining a path and calling the `clip()` function. All following drawing operations will now be clipped by this path. The clipping is disabled again by restoring the previous state or by setting the clip region to the whole canvas.

![](../images/qmlBook_9_canvasElement_202104192032_6.png)

## Transformation

The canvas allows you to transform the coordinate system in several ways. This is very similar to the transformation offered by QML items. You have the possibility to `scale`, `rotate`, `translate` the coordinate system. Indifference to QML the transform origin is always the canvas origin. For example to scale a path around its center you would need to translate the canvas origin to the center of the path. It is also possible to apply a more complex transformation using the transform method.

```qml
// transform.qml

import QtQuick 2.5

Canvas {
    id: root
    width: 240; height: 120
    onPaint: {
            var ctx = getContext("2d");
            ctx.lineWidth = 4;
            ctx.strokeStyle = "blue";

            // translate x/y coordinate system
            ctx.translate(root.width/2, root.height/2);

            // draw path
            ctx.beginPath();
            ctx.rect(-20, -20, 40, 40);
            ctx.stroke();

            // rotate coordinate system
            ctx.rotate(Math.PI/4);
            ctx.strokeStyle = "green";

            // draw path
            ctx.beginPath();
            ctx.rect(-20, -20, 40, 40);
            ctx.stroke();
    }
}
```

![](../images/qmlBook_9_canvasElement_202104192032_7.png)

Besides translate the canvas allows also to scale using `scale(x,y)` around x and y-axis, to rotate using `rotate(angle)`, where the angle is given in radius (360 degree = 2*Math.PI) and to use a matrix transformation using the `setTransform(m11, m12, m21, m22, dx, dy)`.

Note:

- To reset any transformation you can call the resetTransform() function to set the transformation matrix back to the identity matrix:

```qml
ctx.resetTransform()
```

## Source code

[source code](https://qmlbook.github.io/assets/ch09-canvas-assets.tgz)

---

- [上一级](README.md)
- 上一篇 -> [《Qml Book》 - Model-View-Delegate](qmlBook_7_modelViewDelegate.md)
- 下一篇 -> [qwt 简单使用](qwt.md)
