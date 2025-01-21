import os
import argparse

def preprocess_shader(file_path, include_dir):
    """
    Preprocesses a GLSL shader, resolving #include directives.
    """
    with open(file_path, 'r') as file:
        lines = file.readlines()
    
    processed_lines = []
    for line in lines:
        if line.startswith("#include"):
            include_file = line.split('"')[1]  # Extract the file name from the include directive
            include_path = os.path.join(include_dir, include_file)
            with open(include_path, 'r') as include:
                processed_lines.append(include.read())  # Include the content of the file
        else:
            processed_lines.append(line)
    
    return ''.join(processed_lines)

def main():
    parser = argparse.ArgumentParser(description="Preprocess GLSL shaders with #include directives.")
    parser.add_argument("shaders", nargs='+', help="List of shader files to preprocess")
    parser.add_argument("-i", "--include_dir", default=".", help="Directory to search for included files")
    parser.add_argument("-o", "--output_dir", default="./processed_shaders", help="Directory to save processed shaders")

    args = parser.parse_args()

    # Ensure the output directory exists
    os.makedirs(args.output_dir, exist_ok=True)

    for shader_path in args.shaders:
        shader_name = os.path.basename(shader_path)  # Get the shader file name
        output_path = os.path.join(args.output_dir, shader_name)  # Determine output file path
        
        # Preprocess the shader
        processed_code = preprocess_shader(shader_path, args.include_dir)
        
        # Write the preprocessed shader to the output directory
        with open(output_path, 'w') as out_file:
            out_file.write(processed_code)
        
        print(f"Processed {shader_path} -> {output_path}")

if __name__ == "__main__":
    main()
