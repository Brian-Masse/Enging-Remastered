from main import EngineRemasteredApp
from main import EngineObject


app = EngineRemasteredApp()


name = "monkey.ply"
scale = (0.5, 0.5, 0.5)
transform = (-1, -1, 0)
color = (64, 49, 89)
monkey = EngineObject( name=name, scale=scale, transform=transform, color=color )


name = "Icosphere.ply"
scale = (1.2, 1.2, 1.2)
transform = (1, 0, 0)
color = (218, 245, 186)
icosphere = EngineObject( name=name, scale=scale, transform=transform, color=color )

app.addObject( monkey )
app.addObject( icosphere )

app.startApp()