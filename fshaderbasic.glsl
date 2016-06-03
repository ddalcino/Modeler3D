// Phong-Blinn Fragment Shader code

// This code is adapted from code presented in Prof. William Thibault's
// CS 4840 class, and in the textbook:
// "Interactive Computer Graphics: A Top-Down Approach with Shader-Based
// OpenGL (6th Edition)" by Angel and Shreiner


// inputs come directly from C++ code
uniform vec4 AmbientProduct;
uniform vec4 DiffuseProduct;
uniform vec4 SpecularProduct;
uniform float Shininess;
uniform vec4 wireframe_color;

// inputs from the vertex shader code
// these are all in eye coords
varying vec3 Light, View, Normal;

void main()
{
    if (wireframe_color.w > 0.0) {
        gl_FragColor = wireframe_color;
    } else {

        vec4 color;
        vec3 L = normalize(Light);
        vec3 V = normalize(View);
        vec3 N = normalize(Normal);
        vec3 H = normalize ( L + V );

        // Compute terms in the illumination equation
        vec4 ambient = AmbientProduct;

        //// two-sided lighting
        //float Kd = max( dot(L, N), -dot(L, N) );
        float Kd = dot(L, N);
        vec4  diffuse = Kd * DiffuseProduct;

        float Ks = pow( max(dot(N, H), -dot(N, H)), Shininess );
        vec4  specular = Ks * SpecularProduct;

        if( dot(L, N) < 0.0 ) {
            specular = vec4(0.0, 0.0, 0.0, 1.0);
        }

        gl_FragColor = ambient + diffuse + specular;
    }
}

//varying vec3 color;

//void main()
//{
//    // Set fragment color
//    gl_FragColor = vec4(0.6, 0.6, 0.6, 1.0);
//}
