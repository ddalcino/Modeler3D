// fshaderlines.glsl: for drawing lines

// inputs come directly from C++ code
uniform vec4 in_color;


void main()
{
  gl_FragColor = in_color;
}
