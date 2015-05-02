local cc = "gcc"
local files = {"core.cpp"} --these need to be in a certain order
local flags = {"-fpic","-shared","-O3","-std=c++11","-I../liblove","-I../liblove/modules","-I/usr/include/lua5.1"}
local out = "../core.so"

function printExec(cmdline)
	print("> "..cmdline)
	local status = os.execute(cmdline)
	if status ~= 0 then
		error("Command failed!")
	end
end

printExec(cc.." "..table.concat(flags," ").." "..table.concat(files," ").." -o "..out)
