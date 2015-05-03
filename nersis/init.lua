local nersis = {}
nersis.VERSION = "Applebutter v1"

local sharedEnding = ({Windows="dll",["OS X"]="dylib"})[love.system.getOS()] or "so"

function nersis.init()
	nersis.dlopen = require "nersis.fucklimitations" --gives us a dlopener that opens with global symbols
	nersis.dlopen("./nersis/core."..sharedEnding)
	nersis.core = require "nersis.core" --require the core library shared object
	nersis.core.hello()
	
	love.run = nersis.core.love_run
end

function nersis.loadModule(name)
	nersis.dlopen("./nersis/"..name.."."..sharedEnding)
	return require("nersis."..name)
end

return nersis
