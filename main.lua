-- lua & love2d prototype of the nersis engine, this is just testing file please ignore --

nersis = require "nersis"

nersis.init()
nersis.loadModule "testmodule"
local states = nersis.loadModule "statemodule"
states.push { --prove you can still do things from the lua side...
	draw = function()
		love.graphics.circle("fill", 32,32, 16)
	end,
	keypressed = function(key)
		print(key.."!")
	end,
}
