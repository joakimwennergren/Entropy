void RoundedRectangleFrame()
{
    // pixel and moust coordinates
	vec2 p = (2.0*gl_FragCoord.xy-inResolution.xy)/inResolution.y; // inUV0;

    // animate the square
	float si = 0.5; // float s = 0.5 - (radius + (thickness / 10));
    float ra = 0.1;

    // draw the square 
	float d = sdRoundSquare( p, si, ra );

    // apply colors to it
	vec4 col = mix(vec4(1.0, 0.0, 0.0, 1.0), inColor0, 1.0-smoothstep(0.0,0.01,abs(d)) );

	outColor = col;
}
