# 《Qml Book》 - Qt Quick Controls 2


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [《Qml Book》 - Qt Quick Controls 2](#qml-book-qt-quick-controls-2)
  - [Introduction to Controls](#introduction-to-controls)
  - [Source code](#source-code)

<!-- /code_chunk_output -->


注：该文档为 《Qml Book》的学习文档，详见 [Qml book - Qt Quick Controls 2](https://qmlbook.github.io/ch06-controls/controls.html)

This chapter introduces how to use Qt Quick Controls 2 to create a user interface built from standard components such as `buttons`, `labels`, `sliders` and so on. We will look at how various styles can be used to move user interfaces between platforms before diving into custom styling.

## Introduction to Controls

Using Qt Quick from scratch gives you primited graphical and interaction elements from which you can build your user interfaces. Using [Qt Quick Controls 2](https://doc.qt.io/qt-5/qtquickcontrols-index.html) you start from a slightly more structured set of controls to build from. The [Qt Quick Controls QML Types](https://doc.qt.io/qt-5/qtquick-controls2-qmlmodule.html) range from simple text labels and buttons to more complex ones such as sliders and dials. These element are handy if you want to create a user interface based on classic interaction patterns as they provide a foundation to stand on.

The Qt Quick Controls 2 comes with a number of [styles](https://doc.qt.io/qt-5/qtquickcontrols2-styles.html) out of the box that are shown in the table below. 

- The Default style is a basic flat style. 
- The Universal style is based on the Microsoft Universal Design Guidelines
- Material is based on Google’s Material Design Guidelines
- Fusion style is a desktop oriented style.

Some of the styles can be tweaked by tweaking the used palette. The Imagine is a style based on image assets, this allows a graphical designer to create a new style without writing any code at all, not even for palette colour codes.

The Qt Quick Controls 2 is available from the `QtQuick.Controls` import module. In this module you will find the basic controls such as buttons, labels, checkboxes, sliders and so on. In addition to these controls, the following modules are also of interest:

|           Module           	|                                                                 Description                                                                	|
|:--------------------------:	|:------------------------------------------------------------------------------------------------------------------------------------------:	|
|      [QtQuick.Controls](https://doc.qt.io/qt-5/qtquickcontrols-index.html)      	|                                                             The basic controls.                                                            	|
|      [QtQuick.Templates](https://doc.qt.io/qt-5/qtquicktemplates2-index.html)     	|                                                Provides the non-visual part of the controls.                                               	|
|       [QtQuick.Dialogs](https://doc.qt.io/qt-5/qtquickdialogs-index.html)      	| Provides standard dialogs for showing messages, picking files, picking colours, and picking fonts, as well as the base for custom dialogs. 	|
| [QtQuick.Controls.Universal](https://doc.qt.io/qt-5/qtquickcontrols2-universal.html) 	|                                                      Universal style theming support.                                                      	|
|  [QtQuick.Controls.Material](https://doc.qt.io/qt-5/qtquickcontrols2-material.html) 	|                                                       Material style theming support.                                                      	|
|      [Qt.labs.calendar](https://doc.qt.io/qt-5/qtlabscalendar-index.html)      	|                                Controls for supporting date picking and other calendar related interactions.                               	|
|      [Qt.labs.platform](https://doc.qt.io/qt-5/qtlabsplatform-index.html)      	| Support for platform native dialogs for common tasks such as picking files, colours, etc, as well as system tray icons and standard paths. 	|

Notice that the `Qt.labs` modules are experimental, meaning that their APIs can have breaking changes between Qt versions.

Note

- The QtQuick.Dialogs module is a Qt Quick Controls 1 module, but it is also the only way to do dialogs without depending on the QtWidgets module. See below for more details.

## Source code

[source code](https://qmlbook.github.io/assets/ch06-controls-assets.tgz)

---

- [上一级](README.md)
- 上一篇 -> [《Qml Book》 - Fluid Elements](qmlBook_5_FluidElements.md)
- 下一篇 -> [qwt 简单使用](qwt.md)
