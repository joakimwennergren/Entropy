#version 450

// Common
#include "common/object_types.glsl"
#include "common/frag_layout.glsl"

// 2D
#include "2d/sdf/primitives.glsl"
#include "2d/primitives/primitives.glsl"

void main()
{
    switch (objectType) {

        // Rounded rectangle frame
        case OBJ_TYPE_ROUNDED_RECTANGLE_FRAME:
            RoundedRectangleFrame();
        break;

        default:
            outColor = vec4(1.0, 0.0, 1.0, 1.0);
        break;

    }
}
