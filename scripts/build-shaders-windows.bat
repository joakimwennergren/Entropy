@echo off

:: Dir where glsl shader resides
set shaders-dir=..\shaders

:: Array with shaders to compile
set shaders-to-compile=static

for %%a in (%shaders-to-compile%) do ( 

    echo Compiling %%a shader..

    if not exist %shaders-dir%\compiled\%%a (
        :: Create output folder for each shader-pair
        md %shaders-dir%\compiled\%%a
    )

    if exist %shaders-dir%\%%a\%%a.vert (
        :: Combine vertex shader
        python shader-combiner.py %shaders-dir%\%%a\%%a.vert -i %shaders-dir% -o %shaders-dir%\combined\%%a
    ) 

    if exist %shaders-dir%\%%a\%%a.frag (
        :: Combine fragment shader
        python shader-combiner.py %shaders-dir%\%%a\%%a.frag -i %shaders-dir% -o %shaders-dir%\combined\%%a
    ) 

    if exist %shaders-dir%\combined\%%a\%%a.vert (
        :: Compile combined vert shader
        %VULKAN_SDK%\Bin\glslc.exe %shaders-dir%\combined\%%a\%%a.vert -o %shaders-dir%\compiled\%%a\%%a_vert.spv
    ) 

    if exist %shaders-dir%\combined\%%a\%%a.frag (
        :: Compile combined vert shader
        %VULKAN_SDK%\Bin\glslc.exe %shaders-dir%\combined\%%a\%%a.frag -o %shaders-dir%\compiled\%%a\%%a_frag.spv
    ) 
)
