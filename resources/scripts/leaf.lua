sprite = Sprite.new("/Users/joakim/Desktop/Entropy-Engine/resources/logo/entropy.png")
sprite:Translate(700.0, -400.0)
sprite:Scale(100.0, 100.0)

x = 200.0

function OnRender()
    sprite:Translate(x, -400.0)
    x = x + 2.0
    print("MOVING Subsprite")
end