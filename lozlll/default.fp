#version 330
 
// Interpolated values from the vertex shaders
in vec2 UV;
uniform int transparency;
 
// Ouput data
out vec3 color;
 
// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
 
void main(){
	vec4 alpha_mod = vec4(0, 0, 0, transparency);
    // Output color = color of the texture at the specified UV
    color = vec4(texture( myTextureSampler, UV ).rgb, 0) + alpha_mod;
	//gl_FragColor = vec4(1.0, 1.0, 1.0, 0);
}