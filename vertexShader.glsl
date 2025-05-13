#version 410 core           // which version and features to use

uniform float angle;        // received from application (updates on idle)
                            // uniform~shared by all for current draw cycle

in  vec3   vertexCoords;    // "vertex attribute" received from application
                            // we sent (x,y) but shader can promote to (x,y,z)
in  vec3   vertexColor;
in  vec3   vertexNorm;


out vec3   fragmentColor;   // color to send to next stage (fragment shader)
                            // should have the same name/type but with "in"
out vec3   fragmentNormal;
out vec4   fragmentCoord;

void main()
{
    // the rotation matrix around Y
    mat4 rotY = mat4(cos(angle),   0,  sin(angle),  0,
                         0,        1,      0,       0,
                    -sin(angle),   0,  cos(angle),  0,
                         0,        0,      0,       1);

    // scale by 2x times
    mat4 scale2X = mat4(2, 0, 0, 0,
                        0, 2, 0, 0,
                        0, 0, 2, 0,
                        0, 0, 0, 1);

    //send actual vertex coord to fragmentShader
    fragmentCoord = scale2X * vec4(vertexCoords, 1);            //will use for some functions in fragmentShader (do not want to special effect to rotate ON the mesh itself)
	                                                                          
    gl_Position = rotY * fragmentCoord;                         // final vertex position (rotate and scale entire mesh)
    

    // send values to the fragment shader
    fragmentColor = vertexColor;                                //pass vertex color to fragment shader
    fragmentNormal = vec3(rotY * vec4(vertexNorm,1));           // so as vertex rotates, normal follows (avoid dark spot on mesh)


}
