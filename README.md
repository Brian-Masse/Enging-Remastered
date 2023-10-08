# **ENGINE - REMASTERED**

## **Overview**

A Low-Level Graphics Renderer built for MacOS using C and C++, packaged in a simple python API. It aims to bridge low level performance with high level convenience.

Engine Remastered is a graphics engine written in ```C/C++```, accessible through a ```python package```. It was designed to fill the space between the explicit control of developing low-level graphics engines and the convenience of working with high-level APIs in the context of macOS rendering. The code base is written in ```C and C++```, using the ```Vulkan SDK``` to communicate with ```Metal``` to gain access to sophisticated, low-level hardware control in MacOS, and exposed through a high-level, minimal ```python API```. This gives users the opportunity to easily create graphic-baed systems, such as game engines and data visualizers, while still leveraging the performance capabilities of the C code base. 

Engine-Remastered was a 12 week independent project conducted as part of the Phillips Academy Andover course CSC600. The project was designed and proposed in collaboration with a faculty advisor, Dr. Nick Zufelt, who also broadly oversaw the development and publish process. It is now independently maintained.

## **Installation**

```
pip install Engine Remastered
```

will install the main C++ library, the python API, pre-made shader files, sample Object data, and sample textures. It intentionally has no external dependencies to make it easier to ship with other software. Instead, it includes all of its necessary dynamic and static libraries in a distributable `_EngineRemastered.so` This file does not specify any of the API, and instead only represents the c++ backend, the python API is accessed with `Engine Remastered`

Currently EngineRemastered requires python>=python3.9

## **First Render**

### **App Initialization**

To create a render first import the python package

```
import EngineRemastered.API.main as ER
```

this will only include relevant python classes / functions, you do not need to manually link any of your frontend code to the c++ code base.

Begin by creating an instance of `EngineWindow`, which will specify the properties for the physical display window.

```

window = ER.EngineWindow(
    size=( 500, 500 ),
    windowName="Engine Remastered Window",
    resizeable=True,          #defaults to true
    windowColor=(0, 0, 0)     #A tuple, specified in RGB=255
)

```

Once you have a window object, pass it into EngineRemasteredApp to create an instance of the EngineRemastered GUI application

```
app = ER.EngineRemasteredApp( window )
```

This call initializes all the information in the backend. Future versions will support more explicit control overt the Pipeline creation and shader files.

Running the code now should work, however nothing is displayed. You have initialized the backend Vulkan process and systems, but have not specified what to do with it yet. We will now create objects to pass into this app and render their output. EngineRemastered could however, be used to simply compute rendered objects without displaying them. If you get a `include error` at this stage, make sure `_EngineRemasteredXXX.so` is in your python `site-packaged` folder. If not, reinstall the package.

### **Object**

Create an EngineRemastered object to display a certain model. This will take a

- `name`: the absolute directory to a `.ply` object file to render
- `scale`: a 3 dimensional tuple used to scale the the model
- `translate`: a 3 dimensional tuple used to offset the model in local coordinate space
- `color`: the RGB=255 color of the object

note- lighting is currently applied to all objects as an infinitely far away point source.

```
    name = "path_to_dir/monkey.ply"
    scale = (1, 1, 1)
    translate = (0, 0, 0)
    color = (255, 61, 61)

    monkey = ER.EngineObject( name, scale, translate, color )
```

Because these are objects, you can reuse and modify them to create duplicate renders.

Now just add this to the application instance

```
app.addObject(monkey)
```

And call `startApp`. Which will present the render onto the window object you specified.

```
app.startApp()
```

Congratulations!! You should now see the object file you specified displayed on screen! You can move around this virtual space using (w) and (s) for front and back movement, and the arrow keys for left, right, up, and down. You just created your first render with Engine Remastered!

![Image](https://github.com/Brian-Masse/Enging-Remastered/blob/main/EngineRemastered/extra/example.png)

## **Future**

- More control over the c++ code from API

  - Manually specify the main loop of the function
  - A system to respond to keyboard inputs
  - specify the creation / destruction of objects
  - interact / modify already built objects
  - being able to specify file relatively as well as absolutely

- Engine Capabilities:

  - Implement modular lighting
  - Create 3D rotational movement
  - Establish an explicit, user-defined coordinate space
  - object interaction / dynamic lighting

- Enable users to write and modify shader files from python code
- Cleaner package install process (removing .API.main)
