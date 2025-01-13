#version 450

#include "common/push_constants.glsl"

#include "common/instance_data.glsl"

// Common
#include "common/object_types.glsl"
#include "common/frag_layout.glsl"

// 2D
#include "2d/sdf/primitives.glsl"
#include "2d/primitives/primitives.glsl"

void main()
{
    switch (instanceBuffer.objects[PushConstants.instanceIndex].type) {

        case OBJ_TYPE_ROUNDED_RECTANGLE:
            RoundedRectangle();
        break;

        default :
        outColor = vec4(1.0, 0.0, 1.0, 1.0);
        break;

    }
}
