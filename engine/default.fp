#version 410
 
uniform float transparency;
uniform float shadow;
uniform int numlights;
uniform vec3 lightcolors[20];
// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
// Interpolated values from the vertex shaders
in vec2 UV;
uniform float entlight;
in float lightamts[20];
out vec4 color;

void main(){
	vec3 lightcolor = vec3(0, 0, 0);
	float lighting = 0;
	if(texture( myTextureSampler, UV ).a == 0.0)
		discard;
	for(int i = 0;i < numlights;i++){
		lighting+=lightamts[i];
	}
	//Go through each light and figure out how much of the color it controls
	for(int i = 0;i < numlights;i++){
		lightcolor+=lightcolors[i] * lightamts[i] / lighting;
	}
	// Output color = color of the texture at the specified UV
	if(entlight != 0)
		color.rgb = texture(myTextureSampler, UV ).rgb * max((1.0 - shadow), entlight);
	else
		color.rgb = mix(texture(myTextureSampler, UV ).rgb, lightcolor, lighting) * max((1.0 - shadow), lighting);
	color.a = texture( myTextureSampler, UV ).a - transparency;
}