local Primitives = {}

Primitives.version = "1.0"

function Primitives.sprite(path)
    return CreateSprite(path)
end

function Primitives.quad()
    return CreateQuad()
end

return Primitives
