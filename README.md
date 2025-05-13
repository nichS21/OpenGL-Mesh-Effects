# OpenGL Mesh Effects

This is an archive of a shader-based OpenGL program, where in you can render a mesh and choose different fragment-based shader effects to view on the mesh.
You can toggle the effect to flow (either backwards or forwards on the mesh, as well as pausing it) as well as change 't' or 'k' values to change how the effect is displayed on the mesh, based on the noise function it uses. See 'main.cpp' and 'fragmentShader.glsl' for more details.

Visit my gallery page [here.](http://www.cs.gettysburg.edu/~stacni01/cs373/Assignment%2011/assignment11.html)


**Please note:** the meshes in this archive were given by Professor [Ivalyo Ilinkin](https://www.gettysburg.edu/academic-programs/computer-science/faculty/employee_detail.dot?empId=04006600720013294&pageTitle=Ivaylo+Ilinkin) of Gettysburg College as handouts for the associated assignment.


## Setup (easiest way)
The project includes everything needed to build and run if using Visual Studio. If it doesn't work straight from the repo, try 
the below steps.

Using Visual Studio 2022, make sure 'Vcpkg,' a C++ dependency manager, is installed with your Visual Studio. Then:

1)  Open a terminal within Visual Studio
2)  type '*vcpkg new --application*'
3)  then '*vcpkg add port freeglut*' and '*vcpkg add port glew*'
4)  enable '*Use Vcpkg Manifest*' under the project properties menu (under vcpkg submenu) 

You should now be able to build and run the application.
