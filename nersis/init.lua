local nersis = {}
nersis.VERSION = "Applebutter v1"

function nersis.init()
	require "nersis.fucklimitations" --loads nersis.core with RTLD_GLOBAL
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
	require("nersis."..name)
end

return nersis
