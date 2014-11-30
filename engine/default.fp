#version 330
 
// Interpolated values from the vertex shaders
layout(location = 0)in vec2 UV;
layout(location = 1)in float entlight;
uniform float transparency;
 
// Ouput data
out vec4 color;
// Darkness for effects
uniform float shadow;
// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
 
void main(){
    // Output color = color of the texture at the specified UV
    color.rgb = texture( myTextureSampler, UV ).rgb * max((1.0 - shadow), entlight);
	color.a = texture( myTextureSampler, UV ).a - transparency;
}