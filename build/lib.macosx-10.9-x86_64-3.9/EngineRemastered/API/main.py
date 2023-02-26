import os
import sys
import pkg_resources

current = os.path.dirname(os.path.realpath(__file__))
parent = os.path.dirname(current)
builds = os.path.dirname(parent)

import _EngineRemastered as _ER

class EngineRemasteredApp:

    def __init__(this):
        #start the app
        # Main/app/GraphicsPipeline/shader.frag

        # one of these will lead to the site packages folder, where we can access the right data
        p = ""
        for path in sys.path:
            if "site-packages" in path:
                p = path
                break

        this.app = _ER.EngineRemasteredApp()
        this.app.setPath( p )
        this.app.initApp()

    def addObject( this, object ):
        this.app.createObj(object.name, "base.png", object.scale[0], object.scale[1], object.scale[1], object.transform[0], object.transform[1], object.transform[2], object.color[0] / 255, object.color[1] / 255, object.color[2] / 255 )

    def startApp(this):
        this.app.runApp()

class EngineObject:
    def __init__(this, name, scale, transform, color):
        this.name = name
        this.scale = scale
        this.transform = transform
        this.color = color
