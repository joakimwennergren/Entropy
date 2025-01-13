local Primitives = {}

Primitives.version = "1.0"

function Primitives.CreateSprite(path)
    return CreateSprite(path)
end

function Primitives.CreateRoundedRectangle(x, y, z, w, h, cr)
    return CreateRoundedRectangle(x, y, z, w, h, cr)
end

function Primitives.CreateCircle(x, y, z, r)
    return CreateCircle(x, y, z, r)
end

return Primitives
