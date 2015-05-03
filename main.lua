-- lua & love2d prototype of the nersis engine, this is just testing file please ignore --

nersis = require "nersis"

nersis.init()
nersis.loadModule "testmodule"
local states = nersis.loadModule "statemodule"
local entity = nersis.loadModule "entitymodule"
local container = entity.newEntityContainer()
local y = 64
local skeleton = entity.registerSkeleton {
	create = function(entity)
		print("create!")
		local body = entity:getBody()
		local circle = love.physics.newCircleShape(16)
		local fixture = love.physics.newFixture(body, circle)
		body:setPosition(64,y)
		y = y+1
	end,
	draw = function(entity)
		love.graphics.circle("fill", -8, -8, 16)
	end,
}
local ent1 = entity.createEntityFromSkeleton(container, skeleton)
local ent2 = entity.createEntityFromSkeleton(container, skeleton)
states.push { --prove you can still do things from the lua side...
	draw = function()
		--love.graphics.circle("fill", 32,32, 16)
		container:draw()
	end,
	update = function(dt)
		container:update()
	end,
	keypressed = function(key)
		print(key.."!")
	end,
}
