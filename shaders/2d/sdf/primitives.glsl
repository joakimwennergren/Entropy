//---------------------------------------------------------
// draw rectangle frame with rounded edges
//---------------------------------------------------------
float roundedFrame (vec2 pos, vec2 uv, vec2 size, float radius, float thickness)
{
    float d = length(max(abs(uv - pos), size) - size) - radius;
    return smoothstep(0.55, 0.45, abs(d / thickness) * 5.0);
}