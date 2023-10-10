-- Leaf class
Leaf = {
	xPosition = 0.0,
	yPosition = 0.0,
	yOffset = 0.0,
	xVelocity = 0.0,
	yVelocity = 0.0,
	time = 0.0,
	mass = 0.0,
	scale = 0.0,
	sprite = nil
}

-- Assign a random leaf sprite
math.randomseed(os.time())

-- Create a new leaf
function Leaf:new(x, y, yOffset, mass, scale)
		self.xPosition = x
		self.yPosition = y
		self.yOffset = yOffset
		self.mass = mass

		local leaves = {"leaf1.png", "leaf2.png", "leaf3.png", "leaf4.png"}

		local random_index = math.random(1, 4)

		-- @todo load textures at start 
		-- then assign the sprite random texture
		-- Maybe async/sync loading of assets?
		self.sprite = CreateSprite(leaves[random_index])

		self.sprite:SetScale(scale, scale)
		self.sprite:SetPosition(x, (y) * -1)

end

for i=1,10 do
	Leaf:new(i * 200.0 + 200.0, 600.0, 500.0, 100.0, 100.0)
end