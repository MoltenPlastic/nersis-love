local nersis = {}
nersis.VERSION = "Applebutter v1"

function nersis.init()
	--install love2d hooks--
	--TODO: ^ that--
	nersis.core = require "nersis.core" --require the core library shared object
	nersis.core.hello()
end

return nersis
