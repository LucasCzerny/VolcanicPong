@echo off

cd ../shaders

for %%f in (*.vert) do (
	if not exist "%%~nf.frag" (
		echo Couldn't find "%%~nf.frag"
		
		pause
		exit
	)
	
	%VULKAN_SDK%\Bin\glslc.exe "%%~nf.vert" -o "%%~nf.vert.spv"
	%VULKAN_SDK%\Bin\glslc.exe "%%~nf.frag" -o "%%~nf.frag.spv"
	
	echo %%~nf shader was successfully compiled
)

echo Done

pause