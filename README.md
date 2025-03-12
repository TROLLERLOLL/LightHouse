# LightHouse Engine
The LightHouse Engine is a 2D Game Framework intended to make creating games fast and fun.<br>
The project is inspired by libraries like Raylib and MonoGame. As I'm very new to C++ expect weird and down right awful design decisions.<br>
As this project is in early development expect **breaking** changes. As of right now you are able to build a Game with it if you provide a way to play audio.<br>
**Currently the Repo might contain non working code for Linux as I'm primarily working on the Windows support**<br>
Not all features are guaranteed to be available in all Graphics backends<br>
Feel free to open Issues or Pull Requests<br>

### Features
 - DirectX 11 and OpenGL 3.3+
 - SpriteBatching with Instanced Sprites
 - Cross-Platform (Windows and Linux (work in progress))
 - Instanced text drawing through the SpriteBatch
 - Software Rendering fallback through DirectX 11 Warp Rendering (Windows 7 and up) 
 ### Planed features
 - Steam Integration
 - Discord Integration
 - Controller support
 - Audio support
 - Improved Linux support
 - Vulkan (Maybe)
 - Android support (Maybe)
 - Custom Shaders
 - TileMap support
 - Single file header (or at least planned Headers)
 - Software Renderer (maybe to support "every" platform and to learn software rendering)
 - Math for basic Game Math
 - File reader and writer (maybe) for save files and config files
 - Bindings for other Languages (C# is currently planned)

 ### How to use the LightHouse Engine
 Here is a simple example of creating a Window, getting the Graphics Object and clearing the screen with a specific color (Cornflower blue)

```cpp
#include <Graphics/Window.h>

int main(int argc, char* argv[]) {
  GameWindow window("LightHouse Engine", 1280, 720, GraphicsBackend::Best);
  Graphics* graphics = window.getGraphics();

  while (window.pollEvents())
    graphics->Clear(0.39, 0.58, 0.93, 1);

  window.close();
  delete graphics;
  return 0;
}
```
The given code will create the following (the "DX11" in the window title represents the current graphics backend, which might be OpenGL for you)


![image](https://github.com/user-attachments/assets/167f4971-df80-47d2-84f3-43588db15d74)

More samples are coming soon
### Requirements
This project relies on DirectX 11 (when used) and OpenGL (when used). It's your responsibility to provide the headers and libraries for this project.
A prepackaged build of the lib may be found [here](https://github.com/TROLLERLOLL/LightHouse/releases/latest)

### Building 
Currently I just use it as a Sub Directory in CMake and link to it in the Game Project

### Licensing 
The LightHouse Engine is currently licensed under [MIT](https://opensource.org/license/mit).<br>
The MIT License requires that you provide proper copyright attribution for the source code.<br>
This can be done by including a link to this repository, my username, and/or a copy of the license text in the credits of your software (if applicable).<br>
If your software does not have credits, you can provide attribution in a license file, or both, within the distributed software.<br>
The attribution must be clear and easily accessible for users to read the copyright information.<br>

This project also uses the following libraries, which are licensed as Public Domain (unlicensed):
- [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)
- [stb_truetype](https://github.com/nothings/stb/blob/master/stb_truetype.h)
  
For any further questions or if the license poses any issues for your project, feel free to reach out.
