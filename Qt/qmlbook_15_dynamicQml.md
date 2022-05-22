<!-- 《Qml Book》 - Dynamic QML -->

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Loading Components Dynamically](#loading-components-dynamically)
  - [Connecting Indirectly](#connecting-indirectly)
  - [Binding Indirectly](#binding-indirectly)
- [Creating and Destroying Objects](#creating-and-destroying-objects)
  - [Dynamically Loading and Instantiating Items](#dynamically-loading-and-instantiating-items)
    - [Incubating Components](#incubating-components)
  - [Dynamically Instantiating Items from Text](#dynamically-instantiating-items-from-text)
  - [Managing Dynamically Created Elements](#managing-dynamically-created-elements)
- [Tracking Dynamic Objects](#tracking-dynamic-objects)
- [Summary](#summary)
- [Source code](#source-code)

<!-- /code_chunk_output -->

注：该文档为 《Qml Book》的学习文档，详见 [Qml book - Dynamic QML](https://qmlbook.github.io/ch15-dynamicqml/dynamicqml.html)

Until now, we have treated QML as a tool for constructing a static set of scenes and navigating between them. Depending on various states and logic rules, a living and dynamic user interface is constructed. By working with QML and JavaScript in a more dynamic manner, the flexibility and possibilities expand even further. Components can be loaded and instantiated at run-time, elements can be destroyed. Dynamically created user interfaces can be saved to disk and later restored.

## Loading Components Dynamically

The easiest way to dynamically load different parts of QML is to use the [Loader](https://doc.qt.io/qt-5/qml-qtquick-loader.html) element. It serves as a placeholder to the item that is being loaded. The item to load is controlled through either the `source` property or the `sourceComponent` property. The former loads the item from a given URL, while the latter instantiates a component.

As the loader serves as a placeholder for the item being loaded, its size depends on the size of the item, and vice versa. If the `Loader` element has a size, either by having set `width` and `height` or through `anchoring`, the loaded item will be given the loader’s size. If the `Loader` has no size, it is resized in accordance to the size of the item being loaded.

The example described below demonstrates how two separate user interface parts can be loaded into the same space using a `Loader` element. The idea is to have a speed dial that can be either digital or analog, as shown in the illustration below. The code surrounding the dial is unaffected by which item that is loaded for the moment.

![](../images/qmlbook_15_dynamicQml_202105072134_2.png)

![](../images/qmlbook_15_dynamicQml_202105072134_1.png)

The first step in the application is to declare a `Loader` element. Notice that the `source` property is left out. This is because the `source` depends on which state the user interface is in.

```qml
Loader {
    id: dialLoader

    anchors.fill: parent
}
```

In the `states` property of the parent of `dialLoader` a set of `PropertyChanges` elements drives the loading of different QML files depending on the `state`. The `source` property happens to be a relative file path in this example, but it can just as well be a full URL, fetching the item over the web.

```qml
    states: [
        State {
            name: "analog"
            PropertyChanges { target: analogButton; color: "green"; }
            PropertyChanges { target: dialLoader; source: "Analog.qml"; }
        },
        State {
            name: "digital"
            PropertyChanges { target: digitalButton; color: "green"; }
            PropertyChanges { target: dialLoader; source: "Digital.qml"; }
        }
    ]
```

In order to make the loaded item come alive, it is `speed` property must be bound to the root `speed` property. This cannot be done as a direct binding as the item not always is loaded and changes over time. Instead, a `Binding` element must be used. The `target` property of the binding is changed every time the `Loader` triggers the `onLoaded` signal.

```qml
    Loader {
        id: dialLoader

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: analogButton.top

        onLoaded: {
            binder.target = dialLoader.item;
        }
    }
    
    Binding {
        id: binder

        property: "speed"
        value: speed
    }
```

The `onLoaded` signal lets the loading QML act when the item has been loaded. In a similar fashion, the QML being loaded can rely on the `Component.onCompleted` signal. This signal is actually available for all components, regardless of how they are loaded. For instance, the root component of an entire application can use it to kick-start itself when the entire user interface has been loaded.

### Connecting Indirectly

When creating QML elements dynamically, you cannot connect to signals using the `onSignalName` approach used for static setup. Instead, the [Connections](https://doc.qt.io/qt-5/qml-qtqml-connections.html) element must be used. It connects to any number of signals of a `target` element.

Having set the `target` property of a `Connections` element, the signals can be connected, as usual, that is, using the `onSignalName` approach. However, by altering the `target` property, different elements can be monitored at different times.

![](../images/qmlbook_15_dynamicQml_202105072134_3.png)

In the example shown above, a user interface consisting of two clickable areas is presented to the user. When either area is clicked, it is flashed using an animation. The left area is shown in the code snippet below. In the `MouseArea`, the `leftClickedAnimation` is triggered, causing the area to flash.

```qml
        Rectangle {
            id: leftRectangle

            width: 290
            height: 200

            color: "green"

            MouseArea {
                id: leftMouseArea
                anchors.fill: parent
                onClicked: leftClickedAnimation.start();
            }

            Text {
                anchors.centerIn: parent
                font.pixelSize: 30
                color: "white"
                text: "Click me!"
            }
        }
```

In addition to the two clickable areas, a `Connections` element is used. This triggers the third animation when the active, i.e. the `target` of the element, is clicked.

```qml
    Connections {
        id: connections
        onClicked: activeClickedAnimation.start();
    }
```

To determine which `MouseArea` to target, two states are defined. Notice that we cannot set the `target` property using a `PropertyChanges` element, as it already contains a `target` property. Instead a `StateChangeScript` is utilized.

```qml
    states: [
        State {
            name: "left"
            StateChangeScript {
                script: connections.target = leftMouseArea
            }
        },
        State {
            name: "right"
            StateChangeScript {
                script: connections.target = rightMouseArea
            }
        }
    ]
```

When trying out the example, it is worth noticing that when multiple signal handlers are used, all are invoked. The execution order of these is, however, undefined.

When creating a `Connections` element without setting the `target` property, the property defaults to `parent`. This means that it has to be explicitly set to `null` to avoid catching signals from the `parent` until the `target` is set. This behavior does make it possible to create custom signal handler components based on a `Connections` element. This way, the code reacting to the signals can be encapsulated and re-used.

In the example below, the `Flasher` component can be put inside any `MouseArea`. When clicked, it triggers an animation, causing the parent to flash. In the same MouseArea the actual task being triggered can also be carried out. This separates the standardized user feedback, i.e. the flashing, from the actual action.

```qml
import QtQuick 2.5

Connections {
 onClicked: {
  // Automatically targets the parent
 }
}
```

To use the Flas`her, simply instantiate a Flasher within each MouseArea, and it all works.

```qml
import QtQuick 2.5

Item {
 // A background flasher that flashes the background of any parent MouseArea
}
```

When using a `Connections` element to monitor the signals of multiple types of target elements, you sometimes find yourself in a situation where the available signals vary between the targets. This results in the `Connections` element outputting run-time errors as signals are missed. To avoid this, the `ignoreUnknownSignal` property can be set to true. This ignores all such errors.

full code:

```qml
import QtQuick 2.5

Rectangle {
    id: container

    width: 600
    height: 400

    color: "white"

    Column {
        anchors.top: parent.top
        anchors.left: parent.left

        spacing: 20

        Rectangle {
            width: 290
            height: 50

            color: "lightGray"

            MouseArea {
                anchors.fill: parent
                onClicked: container.state = "left"
            }

            Text {
                anchors.centerIn: parent
                font.pixelSize: 30
                text: container.state==="left"?"Active":"inactive";
            }
        }

        // M1>>
        Rectangle {
            id: leftRectangle

            width: 290
            height: 200

            color: "green"

            MouseArea {
                id: leftMouseArea
                anchors.fill: parent
                onClicked: leftClickedAnimation.start();
            }

            Text {
                anchors.centerIn: parent
                font.pixelSize: 30
                color: "white"
                text: "Click me!"
            }
        }
        // <<M1
    }

    Column {
        anchors.top: parent.top
        anchors.right: parent.right

        spacing: 20

        Rectangle {
            width: 290
            height: 50

            color: "lightGray"

            MouseArea {
                anchors.fill: parent
                onClicked: container.state = "right"
            }

            Text {
                anchors.centerIn: parent
                font.pixelSize: 30
                text: container.state==="right"?"Active":"inactive";
            }
        }

        Rectangle {
            id: rightRectangle

            width: 290
            height: 200

            color: "blue"

            MouseArea {
                id: rightMouseArea
                anchors.fill: parent
                onClicked: rightClickedAnimation.start();
            }

            Text {
                anchors.centerIn: parent
                font.pixelSize: 30
                color: "white"
                text: "Click me!"
            }
        }
    }

    Text {
        id: activeText

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 50

        font.pixelSize: 30
        color: "red"
        text: "Active area clicked!"

        opacity: 0
    }

    SequentialAnimation {
        id: leftClickedAnimation

        PropertyAction {
            target: leftRectangle
            property: "color"
            value: "white"
        }
        ColorAnimation {
            target: leftRectangle
            property: "color"
            to: "green"
            duration: 3000
        }
    }

    SequentialAnimation {
        id: rightClickedAnimation

        PropertyAction {
            target: rightRectangle
            property: "color"
            value: "white"
        }
        ColorAnimation {
            target: rightRectangle
            property: "color"
            to: "blue"
            duration: 3000
        }
    }

    SequentialAnimation {
        id: activeClickedAnimation

        PropertyAction {
            target: activeText
            property: "opacity"
            value: 1
        }
        PropertyAnimation {
            target: activeText
            property: "opacity"
            to: 0
            duration: 3000
        }
    }

    // M2>>
    Connections {
        id: connections
        onClicked: activeClickedAnimation.start();
    }
    // <<M2

    // M3>>
    states: [
        State {
            name: "left"
            StateChangeScript {
                script: connections.target = leftMouseArea
            }
        },
        State {
            name: "right"
            StateChangeScript {
                script: connections.target = rightMouseArea
            }
        }
    ]
    // <<M3

    Component.onCompleted: {
        state = "left";
    }
}

```

### Binding Indirectly

Just as it is not possible to connect to signals of dynamically created elements directly, nor it is possible to bind properties of a dynamically created element without working with a bridge element. To bind a property of any element, including dynamically created elements, the [Binding](https://doc.qt.io/qt-5/qml-qtqml-binding.html) element is used.

The `Binding` element lets you specify a `target` element, a `property` to bind and a `value` to bind it to. Through using a Binding` element, it is, for instance, possible to bind properties of a dynamically loaded element. This was demonstrated in the introductory example in this chapter, as shown below.

```qml
    Loader {
        id: dialLoader

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: analogButton.top

        onLoaded: {
            binder.target = dialLoader.item;
        }
    }
    
    Binding {
        id: binder

        property: "speed"
        value: speed
    }
```

As the `target` element of a `Binding` not always is set, and perhaps not always has a given property, the `when` property of the `Binding` element can be used to limit the time when the binding is active. For instance, it can be limited to specific modes in the user interface.

The `Binding` element also comes with a `delayed` property. When this property is set to `true` the binding is not propagated to the target until the event queue has been emptied. In high load situations this can serve as an optimization as intermediate values are not pushed to the `target`.

## Creating and Destroying Objects

The `Loader` element makes it possible to populate part of a user interface dynamically. However, the overall structure of the interface is still static. Through JavaScript, it is possible to take one more step and to instantiate QML elements completely dynamically.

Before we dive into the details of creating elements dynamically, we need to understand the workflow. When loading a piece of QML from a file or even over the Internet, a component is created. The component encapsulates the interpreted QML code and can be used to create items. This means that loading a piece of QML code and instantiating items from it is a two-stage process. First, the QML code is parsed into a component. Then the component is used to instantiate actual item objects.

In addition to creating elements from QML code stored in files or on servers, it is also possible to create QML objects directly from text strings containing QML code. The dynamically created items are then treated in a similar fashion once instantiated.

### Dynamically Loading and Instantiating Items

When loading a piece of QML, it is first interpreted as a component. This includes loading dependencies and validating the code. The location of the QML being loaded can be either a local file, a Qt resource, or even a distance network location specified by a URL. This means that the loading time can be everything from instant, for instance, a Qt resource located in RAM without any non-loaded dependencies, to very long, meaning a piece of code located on a slow server with multiple dependencies that need to be loaded.

The status of a component being created can be tracked by it is `status` property. The available values are `Component.Null`, `Component.Loading`, `Component.Ready` and `Component.Error`. The `Null` to `Loading` to `Ready` is the usual flow. At any stage, the `status` can change to `Error`. In that case, the component cannot be used to create new object instances. The `Component.errorString()` function can be used to retrieve a user-readable error description.

When loading components over slow connections, the `progress` property can be of use. It ranges from `0.0`, meaning nothing has been loaded, to `1.0` indicating that all have been loaded. When the component’s `status` changes to `Ready`, the component can be used to instantiate objects. The code below demonstrates how that can be achieved, taking into account the event of the component becoming ready or failing to be created directly, as well as the case where a component is ready slightly later.

```JavaScript
var component;

function createImageObject() {
    component = Qt.createComponent("dynamic-image.qml");
    if (component.status === Component.Ready || component.status === Component.Error) {
        finishCreation();
    } else {
        component.statusChanged.connect(finishCreation);
    }
}

function finishCreation() {
    if (component.status === Component.Ready) {
        var image = component.createObject(root, {"x": 100, "y": 100});
        if (image === null) {
            console.log("Error creating image");
        }
    } else if (component.status === Component.Error) {
        console.log("Error loading component:", component.errorString());
    }
}
```

The code above is kept in a separate JavaScript source file, referenced from the main QML file.

```qml
import QtQuick 2.5
import "create-component.js" as ImageCreator

Item {
    id: root

    width: 1024
    height: 600

    Component.onCompleted: ImageCreator.createImageObject();
}
```

The `createObject` function of a component is used to create object instances, as shown above. This not only applies to dynamically loaded components but also `Component` elements inlined in the QML code. The resulting object can be used in the QML scene like any other object. The only difference is that it does not have an `id`.

The `createObject` function takes two arguments. The first is a `parent` object of the type `Item`. The second is a list of properties and values on the format `{"name": value, "name": value}`. This is demonstrated in the example below. Notice that the properties argument is optional.

```JavaScript
var image = component.createObject(root, {"x": 100, "y": 100});
```

Note

- A dynamically created component instance is not different to an in-line Component element. The in-line Component element also provides functions to instantiate objects dynamically.

#### Incubating Components

When components are created using `createObject` the creation of the object component is blocking. This means that the instantiation of a complex element may block the main thread, causing a visible glitch. Alternatively, complex components may have to be broken down and loaded in stages using `Loader` elements.

To resolve this problem, a component can be instantiated using the `incubateObject` method. This might work just as `createObject` and return an instance immediately, or it may call back when the component is ready. Depending on your exact setup, this may or may not be a good way to solve instantiation related animation glitches.

To use an incubator, simply use it as `createComponent`. However, the returned object is an incubator and not the object instance itself. When the incubator’s status is `Component.Ready`, the object is available through the `object` property of the incubator. All this is shown in the example below:

```qml
function finishCreationg() {
    if (component.status === Component.Ready) {
        var incubator = component.incubateObject(root, {"x": 100, "y": 100});
        if (incubator.status === Component.Ready) {
            var image = incubator.object; // Created at once
        } else {
            incubator.onStatusChanged = function(status) {
                if (status === Component.Ready) {
                    var image = incubator.object; // Created async
                }
            };
        }
    }
}
```

### Dynamically Instantiating Items from Text

Sometimes, it is convenient to be able to instantiate an object from a text string of QML. If nothing else, it is quicker than putting the code in a separate source file. For this, the `Qt.createQmlObject` function is used.

The function takes three arguments: `qml`, `parent` and `filepath`.

- The `qml` argument contains the string of QML code to instantiate.
- The `parent` argument provides a parent object to the newly created object.
- The `filepath` argument is used when reporting any errors from the creation of the object. The result returned from the function is either a new object or `null`.

Warning

- The `createQmlObject` function always returns immediately. For the function to succeed, all the dependencies of the call must be loaded. This means that if the code passed to the function refers to a non-loaded component, the call will fail and return null. To better handle this, the `createComponent` / `createObject` approach must be used.

The objects created using the `Qt.createQmlObject` function resembles any other dynamically created object. That means that it is identical to every other QML object, apart from not having an `id`. In the example below, a new `Rectangle` element is instantiated from in-line QML code when the `root` element has been created.

```qml
import QtQuick 2.5

Item {
    id: root

    width: 1024
    height: 600

    function createItem() {
        Qt.createQmlObject("import QtQuick 2.5; Rectangle { x: 100; y: 100; width: 100; height: 100; color: \"blue\" }", root, "dynamicItem");
    }

    Component.onCompleted: root.createItem();
}
```

### Managing Dynamically Created Elements

Dynamically created objects can be treated as any other object in a QML scene. However, there are some pitfalls that we need to be aware of. The most important is the concept of the creation contexts.

The creation context of a dynamically created object is the context within it is being created. This is not necessarily the same context as the parent exists in. When the creation context is destroyed, so are the bindings concerning the object. This means that it is important to implement the creation of dynamic objects in a place in the code which will be instantiated during the entire lifetime of the objects.

Dynamically created objects can also be dynamically destroyed. When doing this, there is a rule of thumb: never attempt to destroy an object that you have not created. This also includes elements that you have created, but not using a dynamic mechanism such as `Component.createObject` or `createQmlObject`.

An object is destroyed by calling its `destroy` function. The function takes an optional argument which is an integer specifying how many milliseconds the objects shall exist before being destroyed. This is useful too, for instance, let the object complete a final transition.

```qml
item = Qt.createQmlObject(...);
...
item.destroy();
```

Note

- It is possible to destroy an object from within, making it possible to create self-destroying popup windows for instance.

## Tracking Dynamic Objects

Working with dynamic objects, it is often necessary to track the created objects. Another common feature is to be able to store and restore the state of the dynamic objects. Both these tasks are easily handled using a [ListModel](https://doc.qt.io/qt-5/qml-qtqml-models-listmodel.html) that we populate dynamically.

In the example shown below two types of elements, rockets and ufos can be created and moved around by the user. In order to be able to manipulate the entire scene of dynamically created elements, we use a model to track the items.

The model, a `ListModel`, is populated as the items are created. The object reference is tracked alongside the source URL used when instantiating it. The latter is not strictly needed for tracking the objects but will come in handy later.

```qml
import QtQuick 2.5
import "create-object.js" as CreateObject

Item {
    id: root

    ListModel {
        id: objectsModel
    }

    function addUfo() {
        CreateObject.create("ufo.qml", root, itemAdded);
    }

    function addRocket() {
        CreateObject.create("rocket.qml", root, itemAdded);
    }

    function itemAdded(obj, source) {
        objectsModel.append({"obj": obj, "source": source})
    }
```

As you can tell from the example above, the `create-object.js` is a more generalized form of the JavaScript introduced earlier. The `create` method uses three arguments: a source URL, a root element, and a callback to invoke when finished. The callback gets called with two arguments: a reference to the newly created object and the source URL used.

This means that each time `addUfo` or `addRocket` functions are called, the `itemAdded` function will be called when the new object has been created. The latter will append the object reference and source URL to the `objectsModel` model.

The `objectsModel` can be used in many ways. In the example in question, the `clearItems` function relies on it. This function demonstrates two things. First, how to iterate over the model and perform a task, i.e. calling the `destroy` function for each item to remove it. Secondly, it highlights the fact that the model is not updated as objects are destroyed. Instead of removing the model item connected to the object in question, the `obj` property of that model item is set to `null`. To remedy this, the code explicitly has to clear the model item as the objects are removed.

```qml
    function clearItems() {
        while(objectsModel.count > 0) {
            objectsModel.get(0).obj.destroy();
            objectsModel.remove(0);
        }
    }
```

Having a model representing all dynamically created items, it is easy to create a function that serializes the items. In the example code, the serialized information consists of the source URL of each object along its x and y properties. These are the properties that can be altered by the user. The information is used to build an XML document string.

```JavaScript
    function serialize() {
        var res = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<scene>\n";

        for(var ii=0; ii < objectsModel.count; ++ii) {
            var i = objectsModel.get(ii);
            res += "  <item>\n    <source>" + i.source + "</source>\n    <x>" + i.obj.x + "</x>\n    <y>" + i.obj.y + "</y>\n  </item>\n"
        }

        res += "</scene>";

        return res;
    }
```

The XML document string can be used with an [XmlListModel](https://doc.qt.io/qt-5/qml-qtquick-xmllistmodel-xmllistmodel.html) by setting the `xml` property of the model. In the code below, the model is shown along the `deserialize` function. The `deserialize` function kickstarts the deserialization by setting the `dsIndex` to refer to the first item of the model and then invoking the creation of that item. The callback, `dsItemAdded` then sets that `x` and `y` properties of the newly created object. It then updates the index and creates the next object, if any.

```qml
    XmlListModel {
        id: xmlModel
        query: "/scene/item"
        XmlRole { name: "source"; query: "source/string()" }
        XmlRole { name: "x"; query: "x/string()" }
        XmlRole { name: "y"; query: "y/string()" }
    }

    function deserialize() {
        dsIndex = 0;
        CreateObject.create(xmlModel.get(dsIndex).source, root, dsItemAdded);
    }

    function dsItemAdded(obj, source) {
        itemAdded(obj, source);
        obj.x = xmlModel.get(dsIndex).x;
        obj.y = xmlModel.get(dsIndex).y;

        dsIndex ++;

        if (dsIndex < xmlModel.count)
            CreateObject.create(xmlModel.get(dsIndex).source, root, dsItemAdded);
    }

    property int dsIndex
```

The example demonstrates how a model can be used to track created items, and how easy it is to serialize and deserialize such information. This can be used to store a dynamically populated scene such as a set of widgets. In the example, a model was used to track each item.

An alternate solution would be to use the `children` property of the root of a scene to track items. This, however, requires the items themselves to know the source URL to use to re-create them. It also requires the scene to consist only of dynamically created items, to avoid attempting to serialize and later deserialize any statically allocated objects.

## Summary

In this chapter, we have looked at creating QML elements dynamically. This lets us create QML scenes freely, opening the door for user configurable and plug-in based architectures.

The easiest way to dynamically load a QML element is to use a `Loader` element. This acts as a placeholder for the contents being loaded.

For a more dynamic approach, the `Qt.createQmlObject` function can be used to instantiate a string of QML. This approach does, however, have limitations. The full-blown solution is to dynamically create a `Component` using the `Qt.createComponent` function. Objects are then created by calling the `createObject` function of a `Component`.

As bindings and signal connections rely on the existence of an object `id`, or access to the object instantiation, an alternate approach must be used for dynamically created objects. To create a binding, the `Binding` element is used. The `Connections` element makes it possible to connect to signals of a dynamically created object.

One of the challenges of working with dynamically created items is to keep track of them. This can be done using a `ListModel`. By having a model tracking the dynamically created items, it is possible to implement functions for serialization and deserialization, making it possible to store and restore dynamically created scenes.

## Source code

[source code](https://qmlbook.github.io/assets/ch15-dynamicqml-assets.tgz)

---

- [上一级](README.md)
- 上一篇 -> [《Qml Book》 - storage](qmlBook_14_storage.md)
- 下一篇 -> [《Qml Book》 - JavaScript](qmlbook_16_javaScript.md)
