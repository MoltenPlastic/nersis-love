local buildconfig = dofile("../buildconfig.lua")
local buildarch = ... or "linux_x64"
local build = buildconfig.buildoptions[buildarch]

local cc = build.cc
local files = {"module.cpp"} --these need to be in a certain order
local flags = build.flags
for i, v in pairs(buildconfig.includes) do
	flags[#flags+1] = "-I"..v
end

for i, v in pairs(build.includes or {}) do
	flags[#flags+1] = "-I"..v
end

local out = "../statemodule-"..buildarch..build.ending

function printExec(cmdline)
	print("> "..cmdline)
	local result,status,code=os.execute(cmdline)
	if type(result)=="number" and result~=0 then
		error("Command failed with error code "..result)
	elseif not result then
		error("Command failed with "..(status or "error").." code "..code)
	end
end

for i, file in pairs(files) do
	printExec(cc.." -c "..table.concat(flags," ").." "..file.." -o "..file:gsub("%.cpp",".o"))
end

printExec(cc.." -shared "..table.concat(flags," ").." -o "..out.." "..table.concat(files," "):gsub("%.cpp",".o"))
