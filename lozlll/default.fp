#version 330
 
// Interpolated values from the vertex shaders
in vec2 UV;
uniform float transparency;
 
// Ouput data
out vec4 color;
 
// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
 
void main(){
    // Output color = color of the texture at the specified UV
    color = texture( myTextureSampler, UV ).rgba - vec4(0, 0, 0, transparency);
	//gl_FragColor = vec4(1.0, 1.0, 1.0, 0);
}