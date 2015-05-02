local nersis = {}
nersis.VERSION = "Applebutter v1"

local sharedEnding = ({Windows="dll",["OS X"]="dylib"})[love.system.getOS()] or "so"

function nersis.init()
	nersis.dlopen = require "nersis.fucklimitations" --loads nersis.core with RTLD_GLOBAL, and then gives us a dlopener
	nersis.dlopen("./nersis/core."..sharedEnding)
	nersis.core = require "nersis.core" --require the core library shared object
	nersis.core.hello()
	
	for i, v in pairs(nersis.core) do
		if i:sub(1,5) == "love_" then
			--install love2d hook--
			love[i:sub(6)] = v
		end
	end
end

function nersis.loadModule(name)
	nersis.dlopen("./nersis/"..name.."."..sharedEnding)
	require("nersis."..name)
end

return nersis
