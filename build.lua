local libs={
	"nersis/libnersiscore",
	"nersis/libtestmodule",
	"nersis/libfucklimitations",
	"nersis/libstatemodule",
	"nersis/libentitymodule",
}

function printExec(cmdline)
	print("> "..cmdline)
	local result,status,code=os.execute(cmdline)
	if type(result)=="number" and result~=0 then
		error("Command failed with error code "..result)
	elseif not result then
		error("Command failed with "..(status or "error").." code "..code)
	end
end

print("Building...")
for k,v in pairs(libs) do
	printExec("cd "..v.." && lua build.lua")
end