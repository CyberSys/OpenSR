from OpenSR.World import PlanetStyle, SystemStyle, AsteroidStyle, WorldManager, StyleManager
from OpenSR.Engine import ResourceManager, Color

world = WorldManager.instance()
resources = ResourceManager.instance()

def getCacheDATPath(name):
    return resources.datValue(name).replace('\\', '/')

def loadPlanetStylesFromDAT():    
    datRoot = resources.datRoot()
    for p in datRoot['Data']['SE']['Planet']:
        style = PlanetStyle()
    
        style.id = p.name
        style.texture = getCacheDATPath(p['Image'].value).replace('psd', 'png').replace('data', 'ORC')
        try:
            cloud = p['Cloud0'].value.split(',')
            style.cloudSpeed = - float(cloud[0]) * 20
            style.cloud = getCacheDATPath(cloud[1]).replace('.psd', '.png').replace('data', 'ORC')
            style.hasCloud = True
        except:
            style.hasCloud = False
        style.speed = -12
        style.hasRing = False
        style.hasRingBackground = False
        
        try:
            color = p['AtmColor'].value.split(',')
        except:
            color = (255, 255, 255)
                    
        style.ambientColor = Color(int(color[0]) / 255.0, int(color[1]) / 255.0, int(color[2]) / 255.0)
        world.styleManager().addPlanetStyle(style)
        
def loadSystemStylesFromDAT():    
    datRoot = resources.datRoot()
    for p in datRoot['Data']['SE']['Star']:
        style = SystemStyle()
            
        style.id = p.name
        style.starImage = getCacheDATPath(p['Image'].value.split(',')[1])
        style.starAnimation = getCacheDATPath(p['Anim'].value)
        style.color = Color(1.0, 1.0, 1.0)
                
        world.styleManager().addSystemStyle(style)

def loadAsteroidStylesFromDAT():    
    datRoot = resources.datRoot()
    for p in datRoot['Data']['SE']['Asteroid']:
        style = AsteroidStyle()
            
        style.id = p.name
        style.sprite = getCacheDATPath(p['Image'].value)
        style.animated = True
                
        world.styleManager().addAsteroidStyle(style)