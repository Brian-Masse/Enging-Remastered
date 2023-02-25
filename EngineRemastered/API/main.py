import os
import sys

current = os.path.dirname(os.path.realpath(__file__))
parent = os.path.dirname(current)
builds = os.path.dirname(parent)

import EngineRemastered

class EngineRemasteredApp:

    def __init__(this):
        #start the app
        this.app = EngineRemastered.EngineRemasteredApp()
        this.app.initApp()

    def addObject( this, object ):
        this.app.createObj( object.name, object.scale[0], object.scale[1], object.scale[1], object.transform[0], object.transform[1], object.transform[2], object.color[0] / 255, object.color[1] / 255, object.color[2] / 255 )

    def startApp(this):
        this.app.runApp()

class EngineObject:
    def __init__(this, name, scale, transform, color):
        this.name = name
        this.scale = scale
        this.transform = transform
        this.color = color


app = EngineRemasteredApp()

name = "monkey.ply"
scale =  (1, 1, 1)
translate = (-1, 1, 0)
color = (255, 0, 0)

object = EngineObject(name, scale, translate, color)

app.addObject( object )

app.startApp()

