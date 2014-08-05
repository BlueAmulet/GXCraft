#!/usr/bin/lua
local socket = require("socket")
local tcp = assert(socket.connect(os.getenv("WIILOAD"):match("tcp:(.*)"), 1337))

function decode_85(data)
	local a = { data:byte(1,-1) }
	local b = 0
	for i = 1,#a do
		b = (b * 85) + (a[i]-33)
	end
	return math.floor(b/65536)%256,math.floor(b/256)%256,b%256
end

local scr_mode = false
local scr_header = false
local scr_start = 0
local w,h = nil,nil
local data = {}
while true do
	msg, status = tcp:receive("*l")
	if msg ~= nil and scr_mode == false then
		if msg:find("-- START SCREENSHOT --",nil,true) then
			scr_mode = true
			scr_header = false
			data = {}
			print("Receiving screenshot ...")
		else
			print(msg)
		end
	elseif msg ~= nil and scr_mode == true then
		if msg:find("-- END SCREENSHOT --",nil,true) then
			print("Screenshot received (" .. math.ceil(os.time() - scr_start) .. "s), saving ...")
			scr_mode = false
			scr_header = false
			local file = io.open("rawdata.rgb","wb")
			for i = 1,#data do
				file:write(string.char(data[i]))
			end
			file:close()
			os.execute("convert -size " .. w .. "x" .. h .. " -depth 8 rawdata.rgb wii_screenshot.png")
			print("Screenshot saved.")
		elseif scr_header == false then
			scr_header = true
			w, h = msg:match("W: (%d+), H: (%d+)")
			w, h = w + 0, h + 0
			scr_start = os.time()
		else
			local rdata = msg:match(": (.*)"):gsub("(....)~(...)",function(thing,amount)
				return string.rep(thing,amount+0)
			end)
			for pixel in rdata:gmatch("....") do
				local a,b,c = decode_85(pixel)
				data[#data + 1] = a
				data[#data + 1] = b
				data[#data + 1] = c
			end
			local y = tonumber(msg:match("(%d-): ")) + 1
			print("Y = " .. y .. " (" .. math.floor(y/h*100) .. "%)")
		end
	else
		if status == "closed" then
			break
		else
			print("NOMSG " .. status)
		end
	end
end
