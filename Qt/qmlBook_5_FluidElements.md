# 《Qml Book》 - Fluid Elements

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [《Qml Book》 - Fluid Elements](#qml-book-fluid-elements)
  - [States and Transitions](#states-and-transitions)
    - [States](#states)
    - [Transitions](#transitions)

<!-- /code_chunk_output -->

注：该文档为 《Qml Book》的学习文档，详见 [Qml book - Fluid Elements](https://qmlbook.github.io/ch05-fluid/fluid.html)

Till now, we have mostly looked at simple graphical elements and how to arrange and manipulate them. This chapter is about how to control these changes in a way that a value of a property not just changes instantly, it’s more how the value changes over time: an animation. This technology is one of the key foundations for modern slick user interfaces and can be extended with a system to describe your user interface using states and transitions. Each state defines a set of property changes and can be combined with animations on state changes, called transitions.


## States and Transitions

Often parts of a user interface can be described in states. A state defines a set of property changes and can be triggered by a certain condition. Additional these state switches can have a transition attached which defines how these changes should be animated or any additional actions shall be applied. Actions can also be applied when a state is entered.

### States

You define states in QML with the [State](https://doc.qt.io/qt-5/qml-qtquick-state.html) element, which needs to be bound to the `states` array of any item element. A state is identified through a state name and consist, in its simplest form, of a series of property changes on elements. The default state is defined by the initial properties of the element and is named "" (the empty string).

```qml
Item {
    id: root
    states: [
        State {
            name: "go"
            PropertyChanges { ... }
        },
        State {
            name: "stop"
            PropertyChanges { ... }
        }
    ]
}
```

A state is changed by assigning a new state name to the `state` property of the element with the states defined.



Note

- Another way to switch states is using the `when` property of the `State` element. The `when` property can be set to an expression that evaluates to true when the state should be applied.

```qml
Item {
    id: root
    states: [
        ...
    ]

    Button {
        id: goButton
        ...
        onClicked: root.state = "go"
    }
}
```

![](../images/qmlBook_5_FluidElements_202104121044_1.png)

For example, a traffic light might have two signaling lights. The upper one signaling stop with a red color and the lower one signaling go with a green color. In this example, both lights should not shine at the same time. Let’s have a look at the state chart diagram.

![](../images/qmlBook_5_FluidElements_202104121044_2.png)


```qml
import QtQuick 2.5

Rectangle {
    id: root
    width: 150
    height: 250

    property color black: '#1f1f21'
    property color red: '#fc3d39'
    property color green: '#53d769'


    gradient: Gradient {
        GradientStop { position: 0.0; color: "#2ed5fa" }
        GradientStop { position: 1.0; color: "#2467ec" }
    }


    // M1>>
    Rectangle {
        id: light1
        x: 25; y: 15
        width: 100; height: width
        radius: width/2
        color: root.black
        border.color: Qt.lighter(color, 1.1)
    }

    Rectangle {
        id: light2
        x: 25; y: 135
        width: 100; height: width
        radius: width/2
        color: root.black
        border.color: Qt.lighter(color, 1.1)
    }
    // <<M1

    // M2>>
    state: "stop"

    states: [
        State {
            name: "stop"
            PropertyChanges { target: light1; color: root.red }
            PropertyChanges { target: light2; color: root.black }
        },
        State {
            name: "go"
            PropertyChanges { target: light1; color: root.black }
            PropertyChanges { target: light2; color: root.green }
        }
    ]
    // <<M2

    // M3>>
    MouseArea {
        anchors.fill: parent
        onClicked: parent.state = (parent.state == "stop"? "go" : "stop")
    }
    // <<M3

    // M4>>
    transitions: [
        Transition {
   //         from: "stop"; to: "go"
            from: "*"; to: "*"
            ColorAnimation { target: light1; properties: "color"; duration: 2000 }
            ColorAnimation { target: light2; properties: "color"; duration: 2000 }
        }
    ]
    // <<M4

}
```

### Transitions

A series of transitions can be added to every item. A [transition](https://doc.qt.io/qt-5/qml-qtquick-transition.html) is executed by a state change. You can define on which state change a particular transition can be applied using the `from:` and `to:` properties. These two properties act like a filter when the filter is true the transition will be applied. You can also use the wild-cast “*” which means “any state”. For example `from:"*"; to:"*"` means from any state to any other state and is the default value for `from` and `to`, which means the transition is applied to every state switch.

For this example, we would like to animate the color changes when switching state from “go” to “stop”.

We restrict the transition with the from and to properties to filter only the state change from “go” to “stop”. Inside the transition, we add two color animations for each light, which shall animate the property changes defined in the state description.

```qml
 transitions: [
        Transition {
   //         from: "stop"; to: "go"
            from: "*"; to: "*"
            ColorAnimation { target: light1; properties: "color"; duration: 2000 }
            ColorAnimation { target: light2; properties: "color"; duration: 2000 }
        }
    ]
```

You can change the state though clicking the UI. The state is applied immediately and will also change the state while a transition is running. So try to click the UI while the state is in the transition from “stop” to “go”. You will see the change will happen immediately.


![](../images/qmlBook_5_FluidElements_202104121044_5.png)

result:

![](../images/qmlBook_5_FluidElements_202104121044_3.png)

![](../images/qmlBook_5_FluidElements_202104121044_4.png)


---
- [上一级](README.md)
- 上一篇 -> [《Qml book》 - Quick Starter](qmlBook_4_quickStarter.md)
- 下一篇 -> [qwt 简单使用](qwt.md)
