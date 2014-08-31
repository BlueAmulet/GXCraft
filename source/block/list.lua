local args = { ... }
for i = 1,#args do
	local entry,top,base
	for line in io.lines(args[i]) do
		if line:find("registerBlock",nil,true) then
			entry = line:match("registerBlock%((%d+),")
		elseif line:find("getTexture") then
			if line:find("_top") then
				top = { line:match("getTexture%((%d+),%s?(%d+)%)") }
			elseif line:find("_bott") then
			elseif line:find("_side") or base == nil then
				base = { line:match("getTexture%((%d+),%s?(%d+)%)") }
			end
		end
	end
	print("\t[" .. string.format("% 2s",entry) .. "] = {{" .. string.format("% 2s",base[1]) .. "," .. base[2] .. (top ~= nil and ("},{" .. string.format("% 2s",top[1]) .. "," .. top[2]) or "") .. "}}, -- " .. args[i]:gsub("%.c",""))
end
