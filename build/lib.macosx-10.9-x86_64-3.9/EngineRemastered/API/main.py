import os
import sys

current = os.path.dirname(os.path.realpath(__file__))
parent = os.path.dirname(current)
builds = os.path.dirname(parent)

import _EngineRemastered as _ER

class EngineRemasteredApp:

    #start the app
    def __init__(this, window ):

        # one of these will lead to the site packages folder, where we can access the right data
        p = ""
        for path in sys.path:
            if "site-packages" in path:
                p = path
                break

        this.app = _ER.EngineRemasteredApp()
        this.app.setPath( p )


        this.app.setWindow( 
            window.size[0], window.size[1], 
            window.windowName, 
            window.resizeable,
            window.color[0], window.color[1], window.color[2]
        )


        this.app.initApp()

    def addObject( this, object ):
        this.app.createObj(object.name, "base.png", object.scale[0], object.scale[1], object.scale[1], object.transform[0], object.transform[1], object.transform[2], object.color[0] / 255, object.color[1] / 255, object.color[2] / 255, True )

    def startApp(this):
        this.app.runApp()
        

class EngineWindow: 
    def __init__(self, size, windowName = "Engine Remastered Window", resizeable = True, color = (0, 0, 0)):
        self.size = size
        self.windowName = windowName
        self.resizeable = resizeable
        self.color = ( color[0] / 255, color[1] / 255, color[2] / 255)


class EngineObject:
    def __init__(this, name, scale, transform, color):
        this.name = name
        this.scale = scale
        this.transform = transform
        this.color = color




window = EngineWindow( 
    ( 500, 500 ),
    windowName="Test Window",
    resizeable=True,
    color=( 5, 5, 5 )
 )

app = EngineRemasteredApp( window )

name = "/Users/brianmasse/Desktop/monkey.ply"
translate = (0,0,0)
scale = (1, 1, 1)
color = ( 255, 61, 61 )

monkey = EngineObject(name, scale, translate, color)

app.addObject(monkey)

app.startApp()

