--[[
 Generates pre-rerendered block icons for use in the hotbar and block selection
--]]

--love.graphics.setDefaultFilter("nearest", "nearest")

local terrain = love.graphics.newImage("terrain_blocks.png")
local imagedat = love.image.newImageData(32,32)
local sidec = love.graphics.newCanvas(14,15)
local tmp = love.graphics.newCanvas(16,16) -- Prevent bleeding
local topc = love.graphics.newCanvas(28,28)

love.graphics.setBlendMode("replace", "premultiplied")

local torender = {
	[ 7] = {{ 1,1}}, -- bedrock
	[45] = {{ 7,0}}, -- brick
	[26] = {{ 5,4}}, -- cloth_aqua
	[34] = {{13,4}}, -- cloth_black
	[28] = {{ 7,4}}, -- cloth_blue
	[27] = {{ 6,4}}, -- cloth_cyan
	[35] = {{14,4}}, -- cloth_gray
	[25] = {{ 4,4}}, -- cloth_green
	[30] = {{ 8,4}}, -- cloth_indigo
	[24] = {{ 3,4}}, -- cloth_lime
	[32] = {{12,4}}, -- cloth_magenta
	[22] = {{ 1,4}}, -- cloth_orange
	[33] = {{11,4}}, -- cloth_pink
	[29] = {{ 9,4}}, -- cloth_purple
	[21] = {{ 0,4}}, -- cloth_red
	[31] = {{10,4}}, -- cloth_violet
	[36] = {{15,4}}, -- cloth_white
	[23] = {{ 2,4}}, -- cloth_yellow
	[ 4] = {{ 0,1}}, -- cobble
	[ 3] = {{ 2,0}}, -- dirt
	[37] = {{13,0},true}, -- flower
	[20] = {{ 1,3}}, -- glass
	[41] = {{ 8,2},{ 8,1}}, -- gold
	[ 2] = {{ 3,0},{ 0,0}}, -- grass
	[13] = {{ 3,1}}, -- gravel
	[42] = {{ 7,2},{ 7,1}}, -- iron
	[10] = {{14,1}}, -- lava
	[18] = {{ 6,1}}, -- leaves
	[17] = {{ 4,1},{ 5,1}}, -- log
	[48] = {{ 4,2}}, -- mossy
	[39] = {{13,1},true}, -- mushroom
	[49] = {{ 5,2}}, -- obsidian
	[16] = {{ 2,2}}, -- ore_coal
	[14] = {{ 0,2}}, -- ore_gold
	[15] = {{ 1,2}}, -- ore_iron
	[40] = {{12,1},true}, -- redshroom
	[38] = {{12,0},true}, -- rose
	[12] = {{ 2,1}}, -- sand
	[ 6] = {{15,0},true}, -- sapling
	[47] = {{ 3,2},{ 4,0}}, -- shelf
	[43] = {{ 5,0},{ 6,0}}, -- slab
	[19] = {{ 0,3}}, -- sponge
	[ 1] = {{ 1,0}}, -- stone
	[46] = {{ 8,0},{ 9,0}}, -- tnt
	[ 8] = {{14,0}}, -- water
	[ 5] = {{ 4,0}}, -- wood
}

local count = 0
for _ in pairs(torender) do count = count + 1 end
print("Rendering " .. count .. " blocks")

for k,v in pairs(torender) do
	print(k)
	love.graphics.setColor(255,255,255)
	local base = v[1]
	local top = v[2] or base
	imagedat:mapPixel(function() return 0, 0, 0, 0 end)
	sidec:renderTo(love.graphics.clear)
	tmp:renderTo(love.graphics.clear)
	topc:renderTo(love.graphics.clear)

	local quad = love.graphics.newQuad(base[1]*16, base[2]*16, 16, 16, 256, 256)
	if top == true then
		tmp:renderTo(function()
			love.graphics.draw(terrain,quad)
		end)
		tmpdata = tmp:newImageData()
		imagedat:mapPixel(function(x, y) return tmpdata:getPixel(x/2, y/2) end)
	else
		sidec:renderTo(function()
			love.graphics.draw(terrain,quad,0,0,0,14/16,15/16)
		end)

		local quad = love.graphics.newQuad(top[1]*16, top[2]*16, 16, 16, 256, 256)
		tmp:renderTo(function()
			love.graphics.draw(terrain,quad)
		end)
		topc:renderTo(function()
			love.graphics.draw(tmp,14,14,math.rad(-45),28/22,28/22,8,8)
		end)

		-- Scale is odd, so we do this.
		topcdata = topc:newImageData()
		for y = 0,14 do
			local ry
			if y < 7 then
				ry = y * 2
			else
				ry = y * 2 - 1
			end
			for x = 0,27 do
				topcdata:setPixel(x, y, topcdata:getPixel(x, ry))
			end
		end
		topcdata:mapPixel(function() return 0, 0, 0, 0 end, 0, 15, 28, 13)

		-- Shear is odd, so we do this.
		imagedat:paste(topcdata, 2, 1, 0, 0, 28, 15)
		sidecdata = sidec:newImageData()
		local mult1 = 204/255
		local mult2 = 153/255
		for y = 0,14 do
			for x = 0,13 do
				local r,g,b,a = sidecdata:getPixel(x,y)
				imagedat:setPixel(2+x,9+y+math.floor((x+1)*0.5),r*mult1,g*mult1,b*mult1,a)
				imagedat:setPixel(16+x,16+y-math.floor((x+1)*0.5),r*mult2,g*mult2,b*mult2,a)
			end
		end
	end

	-- Save generated image to disk
	local pngdata = imagedat:encode("png"):getString()
	local file = io.open("inv_blocks/bi_" .. k .. ".png", "wb")
	file:write(pngdata)
	file:close()
end

love.event.quit()
