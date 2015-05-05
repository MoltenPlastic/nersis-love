local nersis = {}
nersis.VERSION = "Applebutter v1"

local sharedEnding = ({Windows="dll",["OS X"]="dylib"})[love.system.getOS()] or "so"

package.cpath = package.cpath..";"..love.filesystem.getSaveDirectory().."/?.so"

function nersis.copyShared(name)
	--TODO: Write a better method.
	print("nersis/"..name.."-"..nersis.TRIPLET.."."..sharedEnding)
	local data = love.filesystem.read("nersis/"..name.."-"..nersis.TRIPLET.."."..sharedEnding)
	love.filesystem.createDirectory("nersis")
	love.filesystem.write("nersis/"..name.."."..sharedEnding, data)
end

function nersis.init()
	--create version triplet--
	local os = love.system.getOS():lower()
	if os == "os x" then os = "mac" end
	local arch = require "ffi".arch
	nersis.TRIPLET = os.."_"..arch
	print(nersis.TRIPLET)
	
	nersis.copyShared "fucklimitations"
	nersis.dlopen = require "nersis.fucklimitations" --gives us a dlopener that opens with global symbols
	
	nersis.copyShared "core"
	nersis.dlopen(love.filesystem.getSaveDirectory().."/nersis/core."..sharedEnding)
	nersis.core = require "nersis.core" --require the core library shared object
	nersis.core.hello()
	
	love.run = nersis.core.love_run
end

function nersis.loadModule(name)
	nersis.copyShared(name)
	nersis.dlopen(love.filesystem.getSaveDirectory().."/nersis/"..name.."."..sharedEnding)
	return require("nersis."..name)
end

return nersis
