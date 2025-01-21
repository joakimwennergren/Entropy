#!/bin/bash

# Directory where GLSL shaders reside
shaders_dir="../shaders"

# Array with shaders to compile
shaders_to_compile=("static")

for shader in "${shaders_to_compile[@]}"; do

    echo "Compiling $shader shader..."

    if [ ! -d "$shaders_dir/compiled/$shader" ]; then
        # Create output folder for each shader-pair
        mkdir -p "$shaders_dir/compiled/$shader"
    fi

    if [ -f "$shaders_dir/$shader/$shader.vert" ]; then
        # Combine vertex shader
        python3 shader-combiner.py "$shaders_dir/$shader/$shader.vert" -i "$shaders_dir" -o "$shaders_dir/combined/$shader"
    fi

    if [ -f "$shaders_dir/$shader/$shader.frag" ]; then
        # Combine fragment shader
        python3 shader-combiner.py "$shaders_dir/$shader/$shader.frag" -i "$shaders_dir" -o "$shaders_dir/combined/$shader"
    fi

    if [ -f "$shaders_dir/combined/$shader/$shader.vert" ]; then
        # Compile combined vertex shader
        /Users/joakimwennergren/VulkanSDK/1.3.296.0/macOS/bin/glslc "$shaders_dir/combined/$shader/$shader.vert" -o "$shaders_dir/compiled/$shader/${shader}_vert.spv"
    fi

    if [ -f "$shaders_dir/combined/$shader/$shader.frag" ]; then
        # Compile combined fragment shader
        /Users/joakimwennergren/VulkanSDK/1.3.296.0/macOS/bin/glslc "$shaders_dir/combined/$shader/$shader.frag" -o "$shaders_dir/compiled/$shader/${shader}_frag.spv"
    fi

done