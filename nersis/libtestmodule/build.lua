local cc = "gcc"
local files = {"module.cpp"} --these need to be in a certain order
local flags = {"-c","-fpic","-O2","-std=c++11","-I../liblove","-I../liblove/modules","-I../libnersiscore","-I/usr/include/lua5.1"}
local out = "../testmodule.so"

function printExec(cmdline)
	print("> "..cmdline)
	local status = os.execute(cmdline)
	if status ~= 0 then
		error("Command failed!")
	end
end

for i, file in pairs(files) do
	printExec(cc.." "..table.concat(flags," ").." "..file.." -o "..file:gsub("%.cpp",".o"))
end

printExec(cc.." -shared -fpic -o "..out.." "..table.concat(files," "):gsub("%.cpp",".o"))
