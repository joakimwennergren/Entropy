local Easing = {}

Easing.version = "1.0"

function Easing.sineIn(t)
    return EaseInSine(t)
end

function Easing.sineOut(t)
    return EaseOutSine(t)
end

function Easing.sineInOut(t)
    return EaseInOutSine(t)
end

return Easing
