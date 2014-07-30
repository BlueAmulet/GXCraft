#!/usr/bin/lua
local socket = require("socket")
local tcp = assert(socket.connect(os.getenv("WIILOAD"):match("tcp:(.*)"), 1337))

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
			w, h = tonumber(w), tonumber(h)
			scr_start = os.time()
		else
			local rdata = msg:match(": (.*)"):gsub("(......)%*(...)",function(thing,amount)
				return string.rep(thing,tonumber(amount))
			end)
			for hex in rdata:gmatch("..") do
				data[#data + 1] = tonumber(hex,16)
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
