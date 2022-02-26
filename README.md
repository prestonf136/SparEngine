# Spar Engine
![Spar Engine Logo](https://github.com/prestonf136/SparEngine/blob/main/logo.svg?raw=true)

Spar Engine is a game engine designed to be usable, and dead simple, **(Don't waste time fighting your engine, focus on your code.)**, however
this engine also gives me the ability to learn about game engine architechture as well as allow me to learn more about writing an optimized
vulkan renderer.

This engine is a **2D only engine**, reason being to lower the scope of the engine allowing me to finish and work on it in a shorter timeframe.
This engine puts low spec devices at the top of it's priority, which influence it's design choices, hence, opting for vulkan.

---

### Getting Started

To get started using Spar Engine we'll start by including it.
```cpp
#include "SparEngine.h"
```

This includes all of Spar Engine, althought that is fairly limited as of now, (only featuring a Window and Renderer class)

Start by creating an instance of the Window class.

```cpp
 SparEngine::Window game_window { /* Lets enter our width */ 640, /* Now our height */ 480, /* And finally our window title*/ "Hello Spar Engine!" };
```

And that's it! That wasn't so hard was it?
Now, we need to create an instance of our Renderer class, to you know... render.

```cpp
SparEngine::Renderer renderer { /* We supply our game window here so our render has access to required extensions and to create a vulkan surface */ game_window };
```

This creates our renderer which we'll soon be able to command to draw.

## 404 the rest of the guide is not found (coming soon!)