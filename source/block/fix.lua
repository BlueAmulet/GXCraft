local args = {...}
for i = 1, #args do	
local file, err = io.open(args[i], "rb")
local data = file:read("*a")
file:close()
data = data:gsub("\tdraw","\tRender::draw")
local file, err = io.open(args[i], "wb")
file:write(data)
file:close()
end
