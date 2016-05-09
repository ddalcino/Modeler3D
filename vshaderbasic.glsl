// Phong-Blinn Vertex Shader code

// This code is adapted from code presented in Prof. William Thibault's
// CS 4840 class, and in the textbook:
// "Interactive Computer Graphics: A Top-Down Approach with Shader-Based
// OpenGL (6th Edition)" by Angel and Shreiner

// inputs come from graphics card memory
attribute  vec4 a_position;
//attribute  vec3 vNormal;

// inputs come directly from C++ code
uniform mat4 ModelView;
uniform mat4 Projection;
uniform vec4 LightPosition;
uniform mat4 mvp_matrix;

// outputs that will be read by the fragment shader
varying vec3 Light, View, Normal;

//void main()
//{
//  // Transform vertex  position into eye coordinates
//  vec3 pos = vec4(ModelView * vec4(vPosition,1.0)).xyz;

//  Light = normalize( LightPosition.xyz - pos );
//  View = normalize( -pos );

//  // Transform vertex normal into eye coordinates
//  Normal = vec4(normalize( ModelView*vec4(vNormal, 0.0) )).xyz;

//  gl_Position = Projection * ModelView * vec4(vPosition.xyz,1.0);
//}


//uniform mat4 mvp_matrix;
//uniform vec3 vertex_color;

//attribute vec4 a_position;

//varying vec3 color;
void main()
{
    // Transform vertex  position into eye coordinates
    vec3 pos = vec4(ModelView * a_position).xyz;

    // We don't have normals yet, so fake them:
    vec4 a_normal = vec4(0.7, 0.0, 0.7, 1.0);

    Light = normalize( LightPosition.xyz - pos );
    View = normalize( -pos );

    // Transform vertex normal into eye coordinates
    Normal = vec4(normalize( ModelView*a_normal )).xyz;

    // Calculate vertex position in screen space
    gl_Position = Projection * ModelView * a_position;
    //gl_Position =  mvp_matrix * a_position;

//    // Pass constant color to fragment shader
//    Light = vec3(1.0, 1.0, 1.0);
//    View = vec3(1.0, 1.0, 1.0);
//    Normal = vec3(1.0, 1.0, 1.0);
}

//uniform mat4 mvp_matrix;

//attribute vec4 a_position;
//attribute vec2 a_texcoord;

//varying vec2 v_texcoord;

////! [0]
//void main()
//{
//    // Calculate vertex position in screen space
//    gl_Position = mvp_matrix * a_position;

//    // Pass texture coordinate to fragment shader
//    // Value will be automatically interpolated to fragments inside polygon faces
//    v_texcoord = a_texcoord;
//}
////! [0]

