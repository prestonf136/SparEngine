# Spar Engine
![Spar Engine Logo](https://github.com/prestonf136/SparEngine/blob/main/logo.svg?raw=true)

Spar Engine is a game engine designed to be usable, and dead simple, **(Don't waste time fighting your engine, focus on your code.)**, however
this engine also gives me the ability to learn about game engine architechture as well as allow me to learn more about writing an optimized
vulkan renderer.

This engine is a **2D only engine**, reason being to lower the scope of the engine allowing me to finish and work on it in a shorter timeframe.
This engine puts low spec devices at the top of it's priority, which influence it's design choices, hence, opting for vulkan.

---
## TODO
 - [x] Window class
 - [ ] Work on render Class

### Getting Started

To get started using Spar Engine we'll start by including it.
```cpp
#include "SparEngine/SparEngine.h"
```

This includes all of Spar Engine, althought that is fairly limited as of now, (only featuring a Window and Renderer class)

Start by creating an instance of the Window class.

```cpp
SparEngine::Window game_window { /* Lets enter our width */ 640, /* Now our height */ 480, /* And finally our window title*/ "Hello Spar Engine!" };
```

And that's it! That wasn't so hard was it?
Now, we need to create an instance of our Renderer class, to you know... render.

```cpp
SparEngine::Renderer renderer { /* we supply the renderer with the game window */ game_window };
```

The reason we supply our game window to our renderer is so that it 
has access to required extensions and to create a vulkan surface 

This creates our renderer which we'll soon be able to command to draw.

Now we don't use this renderer just yet, however it's good to have created it.
we can finally start our window loop

```cpp
game_window.loop();
```

Putting this all together

```cpp
try {
	SparEngine::Window game_window { 640, 480, "Hello Spar Engine!" };
	SparEngine::Renderer renderer { game_window };
	
	game_window.loop();
} 
catch(std::exception err)
{
	// handle our error...
}
```

### Cmake
When using Cmake, simply add the Spar Engine folder as subdirectory:
```cmake
add_subdirectory("SparEngine")
```

and link with the SparEngine library.
```cmake
target_link_libraries([Your App Name] SparEngine::SparEngine)
```

---
Happy Coding!