#version 410 core

uniform int currFunc;           // global variable that determines which function to use
uniform float f;                // user specified value, used by noise functions
uniform int k;                  // user specified value, used in functions 7 & 8
uniform float t;                // user specified value, used by function 8 to determine cap on noise values
uniform int frame;              // frame value used to perturb coordinates to enable effect flow
uniform bool flow;              // flow enable or disable for effects

in  vec3   fragmentColor;       // interpolated color from vertex shader (same name as out variable)
in  vec3   fragmentNormal;
in  vec4   fragmentCoord;        

out vec3   finalColor;          // final color to use for drawing


//method signatures for noise functions
float snoise(vec3 v);
float snoisegrad(vec3 v, out vec3 gradient);
float turbulence(vec3 v, int k);


//returns fragment coords perturbed by increasing frame (moves coords if flow enabled)
void perturbCoords(out vec3 coords)
{
    //even if static (no flow), keep effect where it stopped; user has option to reset frame on application side if desire
    coords = coords + (frame * 1e-5 * vec3(1,1,1)); 
}


//Identity function, returns the color passed in to fragmentShader.glsl
void function0(out vec3 newColor, out vec3 newNormal)
{
    newColor = fragmentColor;
    newNormal = fragmentNormal;
}


//divides range [-1,1] into 7 vertical portions: red, green, blue yellow, magenta, cyan, white
void function1(out vec3 newColor, out vec3 newNormal)
{
    vec3 colors[8] = { vec3(1,0,0), vec3(0,1,0), vec3(0,0,1), 
                       vec3(1,1,0), vec3(1,0,1), vec3(0,1,1),
                       vec3(1,1,1), vec3(1,1,1)};

    vec3 currCoord = fragmentCoord.xyz;
    perturbCoords(currCoord);                   //perturb coords before use

    float x = mod(currCoord.x + 1.0, 2);                // [-1,1] => [0,2] range for x and modulus to keep in a range of 2
    float binWidth = (1 - (-1)) / 7.0;
  
    int index = int(x/binWidth);

    newColor = colors[index];
    newNormal = fragmentNormal;
}


//divide [-1,1] range in alternating strips of .1 and .05 width, not rendering the thinner portion
void function2(out vec3 newColor, out vec3 newNormal)
{
    vec3 currCoord = fragmentCoord.xyz;
    perturbCoords(currCoord);           //perturb coords before use

    float x = currCoord.x + 1.0;            // [-1,1] => [0,2] range
    float binWidth = .15;        

    float remainder = mod(x, binWidth);     //how far into the bin is this x (using modulus)

    if(remainder > .1) discard;             //if greater than .1 into bin (out of .15), then not visible
    else
    {
        newColor = fragmentColor;
        newNormal = fragmentNormal;
    }
}


//gives the mesh a corregated effect
void function3(out vec3 newColor, out vec3 newNormal)
{
    //f of 90 seems to give good results
    vec3 currCoord = fragmentCoord.xyz;
    perturbCoords(currCoord);              //perturb coords before use

    float x = currCoord.x;
    vec3 u = vec3(cos(f*x), sin(f*x), 0);

    newColor = fragmentColor;
    newNormal = fragmentNormal + u;             //perturb actual normal with u
}


//computes noise at f*fragmentCoords and scaled color white by noise
void function4(out vec3 newColor, out vec3 newNormal)
{
    //value of 15 for f seems to give good results
    vec3 currCoord = fragmentCoord.xyz;
    perturbCoords(currCoord);                   //perturb coords before use

    float noise = snoise(f * currCoord);        //[-1,1] range
    noise = (noise + 1) / 2;                    //[0,1] range since used for color


    newColor = noise * vec3(1,1,1);            //scales white by noise
    newNormal = fragmentNormal;
}


//computes noise and gradient at f*fragmentCoords
void function5(out vec3 newColor, out vec3 newNormal)
{
    //f=15 seems to be good value
    vec3 currCoord = fragmentCoord.xyz;                     //demote fragmentCoord to vec3
    perturbCoords(currCoord);                               //perturb coords before use


    float noise = snoise(f * currCoord);                    //[-1,1] range
    noise = (noise + 1) / 2;                                //[0,1] range since used for color

    vec3 gradient;
    float gradNoise = snoisegrad(f*currCoord, gradient);    //[-1,1] range

    newColor = vec3(1,1,1);
    newNormal = fragmentNormal + gradNoise * gradient;
}


//similar to function1, but uses adjusted noise, not x-coord to determine color bin
void function6(out vec3 newColor, out vec3 newNormal)
{
    //f = 10 gives a good effect
    vec3 colors[8] = { vec3(1,0,0), vec3(0,1,0), vec3(0,0,1), 
                       vec3(1,1,0), vec3(1,0,1), vec3(0,1,1),
                       vec3(1,1,1), vec3(1,1,1)};

    vec3 currCoord = fragmentCoord.xyz;                     //demote fragmentCoord to vec3
    perturbCoords(currCoord);                               //perturb coords before use

    float noise = snoise(f*currCoord);

    noise = noise + 1;                                      // [-1,1] => [0,2] range
    float binWidth = (1 - (-1)) / 7.0;
  
    int index = int(noise/binWidth);

    newColor = colors[index];
    newNormal = fragmentNormal;
}


//similar to function4, but use turbulence noise to compute final color instead
void function7(out vec3 newColor, out vec3 newNormal)
{
    //f = 7; k = 3; for decent appearance
    vec3 currCoord = fragmentCoord.xyz;         //demote fragmentCoord to vec3
    perturbCoords(currCoord);                   //perturb coords before use

    float noise = turbulence(f * currCoord, k);        //[0,2] range

    newColor = noise * vec3(1,1,1);            //scales white by noise
    newNormal = fragmentNormal;
}


//like function7, but only turbulence noise values in range [0,..t] are considered (t <= 1), rest are set to 1
void function8(out vec3 newColor, out vec3 newNormal)
{
    //f = 7; k = 3; t = 0.1 for decent appearance
    vec3 currCoord = fragmentCoord.xyz;         //demote fragmentCoord to vec3
    perturbCoords(currCoord);                   //perturb coords before use

    float noise = turbulence(f * currCoord, k);        //[0,2] range
    noise = noise / 2;                                 //[0,1] range since used for color

    //see if in range t
    if(noise > t || noise < 0) noise = 1;
    else noise = noise / t;


    newColor = noise * vec3(1,1,1);            //scales white by noise
    newNormal = fragmentNormal;
}

//function for computing color with light at (0, 10, -10) and diffuse coefficient of 1
void computeFinalColor(vec3 currColor, vec3 currNormal)
{
    vec3 light = vec3(0, 10, -10);

    // vertex to light (normalized)
    vec3 L  = normalize(light - fragmentCoord.xyz);


    //formula for diffuse color only
    finalColor = 1 * vec3(1,1,1) * currColor * dot(normalize(currNormal), L);
}


void main()
{
    vec3 color;
    vec3 normal;

    //choose function to use
    switch(currFunc)
    {
        case 0:
            function0(color, normal);
            break;

        case 1:
            function1(color, normal);
            break;

        case 2:
            function2(color, normal);
            break;

        case 3:
            function3(color, normal);
            break;

        case 4:
            function4(color, normal);
            break;

        case 5:
            function5(color, normal);
            break;

        case 6:
            function6(color, normal);
            break;

        case 7:
            function7(color, normal);
            break;

        case 8:
            function8(color, normal);
            break;

        default:
            break;
    }


    //diffuse color
    computeFinalColor(color, normal);    
}







//------GLSL Noise functions that I have borrowed from the class handouts------//

//
// Description : Turbulence functions based on Ken Perlin's paper
//

float turbulence(vec3 v, int k)
{
  float t = 0;
  int scale = 1;
  while (k > 0) {
    t = t + abs(snoise(v * scale) / scale);
    scale *= 2;
    --k;
  }
  return t;
}

//
// Description : Array and textureless GLSL 2D/3D/4D simplex 
//               noise functions.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : stegu
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
//               https://github.com/stegu/webgl-noise
// 

vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 mod289(vec4 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec4 permute(vec4 x) {
     return mod289(((x*34.0)+1.0)*x);
}

vec4 taylorInvSqrt(vec4 r)
{
  return 1.79284291400159 - 0.85373472095314 * r;
}

float snoise(vec3 v)
{ 
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

// Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //   x0 = x0 - 0.0 + 0.0 * C.xxx;
  //   x1 = x0 - i1  + 1.0 * C.xxx;
  //   x2 = x0 - i2  + 2.0 * C.xxx;
  //   x3 = x0 - 1.0 + 3.0 * C.xxx;
  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
  vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

// Permutations
  i = mod289(i); 
  vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// Gradients: 7x7 points over a square, mapped onto an octahedron.
// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
  float n_ = 0.142857142857; // 1.0/7.0
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
  //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

//Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

// Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1), 
                                dot(p2,x2), dot(p3,x3) ) );
}

float snoisegrad(vec3 v, out vec3 gradient)
{
  const vec2  C = vec2(1.0/6.0, 1.0/3.0) ;
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

// First corner
  vec3 i  = floor(v + dot(v, C.yyy) );
  vec3 x0 =   v - i + dot(i, C.xxx) ;

// Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //   x0 = x0 - 0.0 + 0.0 * C.xxx;
  //   x1 = x0 - i1  + 1.0 * C.xxx;
  //   x2 = x0 - i2  + 2.0 * C.xxx;
  //   x3 = x0 - 1.0 + 3.0 * C.xxx;
  vec3 x1 = x0 - i1 + C.xxx;
  vec3 x2 = x0 - i2 + C.yyy; // 2.0*C.x = 1/3 = C.y
  vec3 x3 = x0 - D.yyy;      // -1.0+3.0*C.x = -0.5 = -D.y

// Permutations
  i = mod289(i); 
  vec4 p = permute( permute( permute( 
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 )) 
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

// Gradients: 7x7 points over a square, mapped onto an octahedron.
// The ring size 17*17 = 289 is close to a multiple of 49 (49*6 = 294)
  float n_ = 0.142857142857; // 1.0/7.0
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z * ns.z);  //  mod(p,7*7)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
  //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

//Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

// Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  vec4 m2 = m * m;
  vec4 m4 = m2 * m2;
  vec4 pdotx = vec4(dot(p0,x0), dot(p1,x1), dot(p2,x2), dot(p3,x3));

// Determine noise gradient
  vec4 temp = m2 * m * pdotx;
  gradient = -8.0 * (temp.x * x0 + temp.y * x1 + temp.z * x2 + temp.w * x3);
  gradient += m4.x * p0 + m4.y * p1 + m4.z * p2 + m4.w * p3;
  gradient *= 42.0;

  return 42.0 * dot(m4, pdotx);
}