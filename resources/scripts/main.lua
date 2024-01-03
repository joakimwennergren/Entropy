local sprite = Sprite.new("/Users/joakim/Desktop/Entropy-Engine/resources/logo/entropy.png")
sprite:Translate(400.0, -400.0)
sprite:Scale(100.0, 100.0)
sprite:SetScriptFile("/Users/joakim/Desktop/Entropy-Engine/resources/scripts/leaf.lua")

--local dynBody = DynamicBody.new()

--local mousePos = GetMousePosition()

--function OnRender()

--    sprite:Translate(x, dynBody:GetPosition())
--    x = x + 2.0
--	print("Main.lua")
--end

x = 400.0

function OnRender()
    sprite:Translate(x, -400.0)
    x = x + 2.0
end