del main.obj
del bin\HelloTriangle.ilk
del bin\HelloTriangle.exp
del bin\HelloTriangle.lib
del bin\HelloTriangle.pdb
del bin\HelloTriangle.exe
mkdir bin
@REM cl.exe /I.\ext /Zi /c main.c
cl.exe /I.\ext /Ox /c main.c
@REM link.exe opengl32.lib kernel32.lib user32.lib gdi32.lib glu32.lib main.obj /OUT:bin\HelloTriangle.exe
link.exe /DEBUG opengl32.lib kernel32.lib user32.lib gdi32.lib glu32.lib main.obj /OUT:bin\HelloTriangle.exe
