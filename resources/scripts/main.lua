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

	leaf = {
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
    setmetatable(leaf, self)
    self.__index = self

	-- Set properties
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

	-- Translate & scale the leaf
	Translate2D(self.sprite, x, (y) * -1)
	Scale2D(self.sprite, scale)
	return leaf
end

function Leaf:update(time)
	force = 1.0 * 9.8;

	acceleration = force / self.mass; 
	self.yVelocity = time --acceleration * time;

	local xVelocity = self.yVelocity * 0.8;

	local height = 840

	print(time)

	--leaf.sprite.SetRotation(this.yVelocity * 0.5, 3);
	Translate2D(self.sprite, 100.0, 100.0)

end

--for i=1,10 do
--	Leaf:new(i * 200.0 + 200.0, 600.0, 500.0, 100.0, 100.0)
--end

-------------------------------------------------------------------------

leaves = {}

for i = 1,20,1 
do 
   leaf = Leaf:new(100.0, 600.0, 0.0, 100.0, 100.0) 
   table.insert(leaves, leaf)
end

time = 0.0

function onRender()
	time = time + 0.03
	leaves[1]:update(time)
end
