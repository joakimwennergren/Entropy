//---------------------------------------------------------
// draw rectangle frame with rounded edges
//---------------------------------------------------------
float roundedFrame (vec2 pos, vec2 uv, vec2 size, float radius, float thickness)
{
    float d = length(max(abs(uv - pos), size) - size) - radius;
    return smoothstep(0.55, 0.45, abs(d / thickness) * 5.0);
}

// s = side length
// r = corner radius
float sdRoundSquare(in vec2 p, in float s, in float r)
{
    vec2 q = abs(p)-s+r;
    return min(max(q.x, q.y), 0.0) + length(max(q, 0.0)) - r;
}

float RectSDF(vec2 p, vec2 b, float r)
{
    vec2 d = abs(p) - b + vec2(r);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0)) - r;
}

float roundedBoxSDF(vec2 CenterPosition, vec2 Size, vec4 Radius)
{
    Radius.xy = (CenterPosition.x > 0.0) ? Radius.xy : Radius.zw;
    Radius.x  = (CenterPosition.y > 0.0) ? Radius.x  : Radius.y;

    vec2 q = abs(CenterPosition)-Size+Radius.x;
    return min(max(q.x, q.y), 0.0) + length(max(q, 0.0)) - Radius.x;
}