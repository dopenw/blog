# 《Qml Book》 - Particle Simulations

<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->
- [《Qml Book》 - Particle Simulations](#qml-book-particle-simulations)
- [《Qml Book》 - Particle Simulations](#qml-book-particle-simulations)
  - [Concept](#concept)
  - [Simple Simulation](#simple-simulation)
  - [Particle Parameters](#particle-parameters)
  - [Directed Particles](#directed-particles)
  - [Source code](#source-code)
  - [Source code](#source-code)
<!-- /code_chunk_output -->

注：该文档为 《Qml Book》的学习文档，详见 [Qml book - Particle Simulations](https://qmlbook.github.io/ch10-particles/particles.html)

Particles are a computer graphics techniques to visualize certain graphics effects. Typical effects could be falling leaves, fire, explosions, meteors, clouds, etc.

It differs from other graphics rendering as particles rendering is based on fuzzy aspects. The outcome is not exactly predictable on pixel-base. Parameters to the particle system describe the boundaries for the stochastic simulation. The phenomena rendered with particles is often difficult to visualize with traditional rendering techniques. The good thing is you can let QML elements interact with the particles systems. Also as parameters are expressed as properties they can be animated using the traditional animation techniques.

## Concept

In the heart of the particle simulation is the [ParticleSystem](https://doc.qt.io/qt-5/qml-qtquick-particles-particlesystem.html) which controls the shared timeline. A scene can have several particles systems, each of them with an independent time-line. A particle is emitted using an [Emitter](https://doc.qt.io/qt-5/qml-qtquick-particles-emitter.html) element and visualized with a [ParticlePainter](https://doc.qt.io/qt-5/qml-qtquick-particles-particlepainter.html), which can be an image, QML item or a shader item. An emitter provides also the direction for particle using a vector space. Particle ones emitted can’t be manipulated by the emitter anymore. The particle module provides the [Affector](https://doc.qt.io/qt-5/qml-qtquick-particles-affector.html), which allows manipulating parameters of the particle after it has been emitted.

Particles in a system can share timed transitions using the [ParticleGroup](https://doc.qt.io/qt-5/qml-qtquick-particles-particlegroup.html) element. By default, every particle is on the empty (‘’) group.

![](../images/qmlBook_10_particleSimulations_202104212156_1.png)

- [ParticleSystem](https://doc.qt.io/qt-5/qml-qtquick-particles-particlesystem.html) - manages shared time-line between emitters
- [Emitter](https://doc.qt.io/qt-5/qml-qtquick-particles-emitter.html) - emits logical particles into the system
- [ParticlePainter](https://doc.qt.io/qt-5/qml-qtquick-particles-particlepainter.html) - particles are visualized by a particle painter
- [Direction](https://doc.qt.io/qt-5/qml-qtquick-particles-direction.html) - vector space for emitted particles
- [ParticleGroup](https://doc.qt.io/qt-5/qml-qtquick-particles-particlegroup.html) - every particle is a member of a group
- [Affector](https://doc.qt.io/qt-5/qml-qtquick-particles-affector.html) - manipulates particles after they have been emitted

## Simple Simulation

Let us have a look at a very simple simulation to get started. Qt Quick makes it actually very simple to get started with particle rendering. For this we need:

- A `ParticleSystem` which binds all elements to a simulation
- An `Emitter` which emits particles into the system
- A `ParticlePainter` derived element, which visualizes the particles

```qml
import QtQuick 2.5
import QtQuick.Particles 2.0

Rectangle {
    id: root
    width: 480; height: 160
    color: "#1f1f1f"

    ParticleSystem {
        id: particleSystem
    }

    Emitter {
        id: emitter
        anchors.centerIn: parent
        width: 160; height: 80
        system: particleSystem
        emitRate: 10
        lifeSpan: 1000
        lifeSpanVariation: 500
        size: 16
        endSize: 32
        Tracer { color: 'green' }
    }

    ImageParticle {
        source: "assets/particle.png"
        system: particleSystem
    }
}
```

![](../images/qmlBook_10_particleSimulations_202104212012_1.gif)

We start with dark rectangle as our root element and background. Therein we declare a `ParticleSystem`. This is always the first step as the system binds all other elements together. Typically the next element is the `Emitter`, which defines the emitting area based on it’s bounding box and basic parameters for them to be emitted particles. The emitter is bound to the system using the `system` property.

The emitter in this example emits 10 particles per second (`emitRate: 10`) over the area of the emitter with each a lifespan of 1000msec (`lifeSpan : 1000`) and a lifespan variation between emitted particles of 500 msec (`lifeSpanVariation: 500`). A particle shall start with a size of 16px (`size: 16`) and at the end of its life shall be 32px (`endSize: 32`).

The green bordered rectangle is a [tracer](https://doc-snapshots.qt.io/neptune3ui/qml-tracer.html) element to show the geometry of the emitter. This visualizes that also while the particles are emitted inside the emitters bounding box the rendering is not limited to the emitters bounding box. The rendering position depends upon life-span and direction of the particle. This will get more clear when we look into how to change the direction particles.

The emitter emits logical particles. A logical particle is visualized using a `ParticlePainter` in this example we use an [ImageParticle](https://doc.qt.io/qt-5/qml-qtquick-particles-imageparticle.html), which takes an image URL as the source property. The image particle has also several other properties, which control the appearance of the average particle.

- `emitRate`: particles emitted per second (defaults to 10 per second)
- `lifeSpan`: milliseconds the particle should last for (defaults to 1000 msec)
- `size, endSize`: size of the particles at the beginning and end of their life (defaults to 16 px)

Changing these properties can influence the result in a drastically way:

```qml
    Emitter {
        id: emitter
        anchors.centerIn: parent
        width: 20; height: 20
        system: particleSystem
        emitRate: 40
        lifeSpan: 2000
        lifeSpanVariation: 500
        size: 64
        sizeVariation: 32
        Tracer { color: 'green' }
    }
```

Besides increasing the emit rate to 40 and the lifespan to 2 seconds the size now starts at 64 pixels and decreases 32 pixels at the end of a particle lifespan.

Increasing the `endSize` even more would lead to a more or less white background. Please note also when the particles are only emitted in the area defined by the emitter the rendering is not constrained to it.

![](../images/qmlBook_10_particleSimulations_202104212012_2.gif)

Increasing the `endSize` even more would lead to a more or less white background. Please note also when the particles are only emitted in the area defined by the emitter the rendering is not constrained to it.

## Particle Parameters

We saw already how to change the behavior of the emitter to change our simulation. The particle painter used allows us how the particle image is visualized for each particle.

```qml
import QtQuick 2.5
import QtQuick.Particles 2.0

Rectangle {
    id: root
    width: 480; height: 160
    color: "#1F1F1F"

    ParticleSystem {
        id: particleSystem
    }

    // M1>>
    ImageParticle {
        source: "assets/star.png"
        system: particleSystem
        // The particle shall be colorized in an gold color 
        //which varies from particle to particle by +/- 20%:
        color: '#FFD700'
        colorVariation: 0.2
        rotation: 0
        rotationVariation: 45
        // 旋转速度
        rotationVelocity: 15
        rotationVelocityVariation: 15
        entryEffect: ImageParticle.Scale
    }
    // <<M1


    Emitter {
        id: emitter
        anchors.fill: parent
        system: particleSystem
        lifeSpan: 8000
        size: 32
        endSize: 16
    }
}
```

![](../images/qmlBook_10_particleSimulations_202104212012_3.gif)

## Directed Particles

We have seen particles can rotate. But particles can also have a trajectory. The trajectory is specified as the velocity or acceleration of particles defined by a stochastic direction also named a vector space.

There are different vector spaces available to define the velocity or acceleration of a particle:
- [AngleDirection](https://doc.qt.io/qt-5/qml-qtquick-particles-angledirection.html) - a direction that varies in angle
- [PointDirection](https://doc.qt.io/qt-5/qml-qtquick-particles-pointdirection.html) - a direction that varies in x and y components
- [TargetDirection](https://doc.qt.io/qt-5/qml-qtquick-particles-targetdirection.html) - a direction towards the target point

![](../images/qmlBook_10_particleSimulations_202104212156_2.png)

Let’s try to move the particles over from the left to the right side of our scene by using the velocity directions.

We first try the `AngleDirection`. For this we need to specify the `AngleDirection` as an element of the velocity property of our emitter.

The angle where the particles are emitted is specified using the angle property. The angle is provided as a value between 0..360 degree and 0 points to the right. For our example, we would like the particles to move to the right so 0 is already the right direction. The particles shall spread by +/- 5 degrees:

The magnitude is defined in pixels per seconds. As we have ca. 640px to travel 100 seems to be a good number. This would mean by an average lifetime of 6.4 secs a particle would cross the open space. To make the traveling of the particles more interesting we vary the magnitude using the `magnitudeVariation` and set this to the half of the magnitude:

```qml
    Emitter {
        id: emitter
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        width: 1; height: 1
        system: particleSystem
        lifeSpan: 6400
        lifeSpanVariation: 400
        size: 32
        velocity: AngleDirection {
            angle: 0
            angleVariation: 15
            magnitude: 100
            magnitudeVariation: 50
        }
    }
```

![](../images/qmlBook_10_particleSimulations_202104212012_4.gif)

## Source code

[source code](https://qmlbook.github.io/assets/ch10-particles-assets.tgz)

---

- [上一级](README.md)
- 上一篇 -> [QML Tutorial](qml.md)
- 下一篇 -> [《Qml Book》 - Meet Qt5](qmlBook_1_MeetQt5.md)
