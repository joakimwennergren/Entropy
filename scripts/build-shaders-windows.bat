@echo off

echo compiling vertex shaders..

for /R %%f in (..\shaders\*.vert) do (
    %VULKAN_SDK%\Bin\glslc.exe ..\shaders\%%~nf.vert -o ..\shaders\compiled\%%~nf_vert.spv
)
echo Done.
echo compiling fragment shaders..

for /R %%f in (..\shaders\*.frag) do (
    %VULKAN_SDK%\Bin\glslc.exe ..\shaders\%%~nf.frag -o ..\shaders\compiled\%%~nf_frag.spv
)

echo Done.
