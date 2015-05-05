-- lua & love2d prototype of the nersis engine, this is just testing file please ignore --

nersis = require "nersis"

nersis.init()
nersis.loadModule "testmodule"
local states = nersis.loadModule "statemodule"
nersis.loadModule "entitymodule"
local container = nersis.entity.EntityContainer()
local y = 128
local ball = nersis.entity.registerSkeleton {
	name = "ball",
	create = function(entity)
		print("create ball!")
		local body = entity:getBody()
		local circle = love.physics.newCircleShape(16)
		local fixture = love.physics.newFixture(body, circle)
	end,
	draw = function(entity)
		love.graphics.circle("fill", 0, 0, 16)
	end,
}
--[[local box = nersis.entity.registerSkeleton {
	name = "box",
	create = function(entity)
		print("create box!")
		local body = entity:getBody()
		local box = love.physics.newRectangleShape(16,16)
		local fixture = love.physics.newFixture(body, box)
	end,
	draw = function(entity)
		love.graphics.rectangle("fill", -8, -8, 16, 16)
	end,
}]]
local spinner = nersis.entity.registerSkeleton {
	name = "spinner",
	create = function(entity)
		print("create spinner!")
		local body = entity:getBody()
		local rectangle = love.physics.newRectangleShape(128,8)
		local fixture = love.physics.newFixture(body, rectangle)
		local mousejoint = love.physics.newMouseJoint(body, 256,128+32+4)
	end,
	draw = function(entity)
		love.graphics.rectangle("fill", -64, -4, 128, 8)
	end,
	update = function(entity)
		entity:getBody():applyAngularImpulse(entity:getData()["speed"])
	end
}
local boxcontainer = nersis.entity.registerSkeleton {
	name = "boxcontainer",
	create = function(entity)
		print("create boxcontainer!")
		local body = entity:getBody()
		local edge1 = love.physics.newEdgeShape(0,0,256,0)
		local fixture1 = love.physics.newFixture(body, edge1)
		local edge2 = love.physics.newEdgeShape(256,0,256,256)
		local fixture2 = love.physics.newFixture(body, edge2)
		local edge3 = love.physics.newEdgeShape(256,256,0,256)
		local fixture3 = love.physics.newFixture(body, edge3)
		local edge4 = love.physics.newEdgeShape(0,256,0,0)
		local fixture4 = love.physics.newFixture(body, edge4)
		--body:setType("static")
		body:setBullet(true)
	end,
	draw = function(entity)
		love.graphics.rectangle("line", 0, 0, 256, 256)
	end
}

local ball1 = nersis.entity.createEntityFromSkeleton(container, ball, 256, y)
local ball2 = nersis.entity.createEntityFromSkeleton(container, ball, 256, y+64)
local spinner = nersis.entity.createEntityFromSkeleton(container, spinner, 256, 128+32+4)
spinner:getData()["speed"] = 100
local boxcontainer = nersis.entity.createEntityFromSkeleton(container, boxcontainer, 128, 128)

states.push { --prove you can still do things from the lua side...
	draw = function()
		--love.graphics.circle("fill", 32,32, 16)
		container:draw()
	end,
	update = function(dt)
		container:update(dt)
	end,
	keypressed = function(key)
		print(key.."!")
		if key == " " then
			local sd = spinner:getData()
			sd.speed = sd.speed*10
		end
	end,
}
