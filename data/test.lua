engine = Rangers.Engine:instance()
resources = Rangers.ResourceManager:instance()

background = resources:getSprite(L"2bg.dds", this)
background:setWidth(background:width()/(engine:screenHeight()/background:height()))
background:setHeight(engine:screenHeight())
background:setLayer(-1)

bgPlanet = resources:getSprite(L"DATA/FormMain3/2Planet.gi", this)
bgPlanet:setOrigin(Rangers.POSITION_X_LEFT, Rangers.POSITION_Y_BOTTOM)
bgPlanet:setPosition(0, engine:screenHeight())
bgPlanet:setLayer(0)

bgAnimLine = resources:getAnimatedSprite(L"DATA/FormMain2/2AnimLine.gai", true, this)
bgAnimLine:setLayer(0)

ship = resources:getAnimatedSprite(L"2ship.dds", true, this)
ship:setPosition(0, engine:screenHeight() - ship:height())
ship:setLayer(1)

t = 0
bgspeed = 5/1000
bgforward = true

engine:focusWidget(this)

function keyPressed(k)
--   bgAnimLine:delete()
--   bgAnimLine = nil
end

function getBoundingRect()
  r = Rangers.Rect:new()
  r.x1 = 0.0
  r.x2 = ship:width()
  r.y1 = 0.0
  r.y2 = ship:height()
  return r
end

function mouseEnter()
end

function mouseLeave()
end

function mouseMove(x, y)
end

function mouseUp(key, x, y)
end

function mouseDown(key, x, y)
end

function mouseClick(x, y)

end

function processLogic(dt)
  if math.abs(t * bgspeed) >= background:width() - engine:screenWidth() then
    t = 0
    bgforward = not bgforward
  end
  if bgforward then
    background:setPosition(- t * bgspeed, 0)
  else 
    background:setPosition(-background:width() + engine:screenWidth() + t * bgspeed, 0)
  end
  t = t + dt
end