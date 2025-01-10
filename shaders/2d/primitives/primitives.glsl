void RoundedRectangleFrame()
{
    vec4 frameColor = vec4(0.0, 0.0, 0.0, 1.0);
    float radius = 0.08;
    float thickness = 0.2;
    float s = 0.5 - (radius + (thickness / 10));
    vec2 size = vec2(s, s);
    vec2 pos = vec2(0.5, 0.5);

    // Normalize the pixel coordinates (this is "passTexCoords" in your case)
    vec2 uv = inUV0;//inUV0.xy/inResolution.xy;

    // (Note: iResolution.xy holds the x and y dimensions of the window in pixels)
    vec2 aspectRatio = vec2(inResolution.x/inResolution.y, 1.0);

    // In order to make sure visual distances are preserved, we multiply everything by aspectRatio
    //uv *= aspectRatio;
    //pos *= aspectRatio;
    //size *= aspectRatio;

    //--- rounded frame ---
    float intensity = roundedFrame(pos, uv, size, radius, thickness);
    vec4 col = mix(vec4(0.0, 0.0, 1.0, 0.0), frameColor, intensity);

    outColor = col;
}