local sprite = Sprite.new("/Users/joakim/Desktop/Entropy-Engine/resources/logo/entropy.png")
sprite:Translate(700.0, -400.0)
sprite:Scale(100.0, 100.0)

sprite.parent:Translate(700.0, -400.0)

function Test()
    print("TEST")
end