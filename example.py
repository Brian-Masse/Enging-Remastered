import EngineRemastered.API.main as ER


app = ER.EngineRemasteredApp()

name = "monkey.ply"
scale = (1, 1, 1)
transform = (0, 0, 0)
color = (255, 0, 0)

object = ER.EngineObject(name, scale, transform, color)

app.addObject( object )

app.startApp()


