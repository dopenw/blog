# 《Qml Book》 - Networking

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [《Qml Book》 - Networking](#qml-book-networking)
  - [Serving UI via HTTP](#serving-ui-via-http)
    - [Networked Components](#networked-components)
  - [Templating](#templating)
  - [Source code](#source-code)

<!-- /code_chunk_output -->

注：该文档为 《Qml Book》的学习文档，详见 [Qml book - Networking](https://qmlbook.github.io/ch13-networking/networking.html)

Qt 5 comes with a rich set of networking classes on the C++ side. There are for example high-level classes on the HTTP protocol layer in a request-reply fashion such as [QNetworkRequest](https://doc.qt.io/qt-5/qnetworkrequest.html), [QNetworkReply](https://doc.qt.io/qt-5/qnetworkreply.html) and [QNetworkAccessManager](https://doc.qt.io/qt-5/qnetworkaccessmanager.html). But also lower levels classes on the TCP/IP or UDP protocol layer such as `QTcpSocket`, `QTcpServer` and `QUdpSocket`. Additional classes exist to manage proxies, network cache and also the systems network configuration.

This chapter will not be about C++ networking, this chapter is about Qt Quick and networking. So how can I connect my QML/JS user interface directly with a network service or how can I serve my user interface via a network service? There are good books and references out there to cover network programming with Qt/C++. Then it is just a manner to read the chapter about C++ integration to come up with an integration layer to feed your data into the Qt Quick world.

## Serving UI via HTTP

To load a simple user interface via HTTP we need to have a web-server, which serves the UI documents. We start off with our own simple web-server using a python one-liner. But first, we need to have our demo user interface. For this, we create a small main.qml file in our project folder and create a red rectangle inside.

```qml
// main.qml
import QtQuick 2.5

Rectangle {
    width: 320
    height: 320
    color: '#ff0000'
}
```

To serve this file we launch a small python script:

```shell
cd <PROJECT>
python3 -m http.server 8080
```

Now our file should be reachable via `http://localhost:8080/main.qml`. You can test it with:

```sh
curl http://localhost:8080/main.qml
```

Or just point your browser to the location. Your browser does not understand QML and will not be able to render the document through. We need to create now such a browser for QML documents. To render the document we need to point our `qmlscene` to the location. Unfortunately the `qmlscene` is limited to local files only. We could overcome this limitation by writing our own `qmlscene` replacement or simple dynamically load it using QML. We choose the dynamic loading as it works just fine. For this, we use a loader element to retrieve for us the remote document.

```qml
// remote.qml
import QtQuick 2.5

Loader {
    id: root
    source: 'http://localhost:8080/main2.qml'
    onLoaded: {
        root.width = item.width
        root.height = item.height
    }
}
```

Now we can ask the `qmlscene` to load the local `remote.qml` loader document. There is one glitch still. The loader will resize to the size of the loaded item. And our qmlscene needs also to adapt to that size. This can be accomplished using the `--resize-to-root` option to the `qmlscene`,

```qml
qmlscene --resize-to-root remote.qml
```

Resize to root tells the qml scene to resize its window to the size of the root element. The remote is now loading the `main.qml` from our local server and resizes itself to the loaded user interface. Sweet and simple.

Note

- If you do not want to run a local server you can also use the gist service from GitHub. The gist is a clipboard like online services like Pastebin and others. It is available under [https://gist.github.com.](https://gist.github.com/discover) I created for this example a small gist under the URL [https://gist.github.com/jryannel/7983492](https://gist.github.com/jryannel/7983492). This will reveal a green rectangle. As the gist URL will provide the website as HTML code we need to attach a `/raw` to the URL to retrieve the raw file and not the HTML code.

```qml
// remote.qml
import QtQuick 2.5

Loader {
    id: root
    source: 'https://gist.github.com/jryannel/7983492/raw'
    onLoaded: {
        root.width = item.width
        root.height = item.height
    }
}
```

To load another file over the network you just need to reference the component name. For example a `Button.qml` can be accessed as normal, as long it is in the same remote folder.

### Networked Components

Let us create a small experiment. We add to our remote side a small button as a reusable component.

- src/main.qml
- src/Button.qml

We modify our `main.qml` to use the button and save it as `main2.qml`:

```qml
import QtQuick 2.5

Rectangle {
    width: 320
    height: 320
    color: '#ff0000'

    Button {
        anchors.centerIn: parent
        text: 'Click Me'
        onClicked: Qt.quit()
    }
}
```

And launch our web-server again

```sh
cd src
python3 -m http.server 8080
```

And our remote loader loads the main QML via HTTP again

```sh
qmlscene --resize-to-root remote.qml
```

What we see is an error

```shell
http://localhost:8080/main2.qml:11:5: Button is not a type
```

So QML cannot resolve the button component when it is loaded remotely. If the code would be local `qmlscene src/main.qml` this would be no issue. Locally Qt can parse the directory and detect which components are available but remotely there is no “list-dir” function for HTTP. We can force QML to load the element using the import statement inside `main.qml`:

```qml
import "http://localhost:8080" as Remote

...

Remote.Button { ... }
```

This will work then when the `qmlscene` is run again:

```qml
qmlscene --resize-to-root remote.qml
```

Here the full code:

```qml
// main2.qml
import QtQuick 2.5
import "http://localhost:8080" 1.0 as Remote

Rectangle {
    width: 320
    height: 320
    color: '#ff0000'

    Remote.Button {
        anchors.centerIn: parent
        text: 'Click Me'
        onClicked: Qt.quit()
    }
}
```

A better option is to use the `qmldir` file on the server side to control the export.

```shell
// qmldir
Button 1.0 Button.qml
```

And then updating the main.qml:

```qml
import "http://localhost:8080" 1.0 as Remote

...

Remote.Button { ... }
```

When using components from a local file system, they are created immediately without a latency. When components are loaded via the network they are created asynchronously. This has the effect that the time of creation is unknown and an element may not yet be fully loaded when others are already completed. Take this into account when working with components loaded over the network.

## Templating

When working with HTML projects they often use template driven development. A small HTML stub is expanded on the server side with code generated by the server using a template mechanism. For example, for a photo list, the list header would be coded in HTML and the dynamic image list would be dynamically generated using a template mechanism. In general, this can also be done using QML but there are some issues with it.

First, it is not necessary. The reason HTML developers are doing this is to overcome limitations on the HTML backend. There is no component model yet in HTML so dynamic aspects have to be covered using these mechanisms or using programmatically javascript on the client side. Many JS frameworks are out there (jQuery, dojo, backbone, angular, …) to solve this issue and put more logic into the client-side browser to connect with a network service. The client would then just use a web-service API (e.g. serving JSON or XML data) to communicate with the server. This seems also the better approach for QML.

The second issue is the component cache from QML. When QML accesses a component it caches the render-tree and just loads the cached version for rendering. A modified version on disk or remote would not be detected without restarting the client. To overcome this issue we could use a trick. We could use URL fragments to load the URL (e.g. [http://localhost:8080/main.qml#1234](http://localhost:8080/main.qml#1234)), where ‘#1234’ is the fragment. The HTTP server serves always the same document but QML would store this document using the full URL, including the fragment. Every time we would access this URL the fragment would need to change and the QML cache would not get a positive hit. A fragment could be for example the current time in milliseconds or a random number.

```qml
Loader {
    source: 'http://localhost:8080/main.qml#' + new Date().getTime()
}
```

In summary templating is possible but not really recommended and does not play to the strength of QML. A better approach is to use web-services which serve JSON or XML data.

## Source code

[source code](https://qmlbook.github.io/assets/ch13-networking-assets.tgz)

---

- [上一级](README.md)
- 上一篇 -> [《Qml Book》 - Multimedia](qmlBook_12_multimedia.md)
- 下一篇 -> [《Qml Book》 - Meet Qt5](qmlBook_1_MeetQt5.md)
