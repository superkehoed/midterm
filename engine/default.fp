#version 410
 
uniform float transparency;
uniform float shadow;
uniform int numlights;
uniform vec3 lightcolors[20];
uniform float lightstrengths[20];
uniform vec3 lightpositions[20];
uniform float lightspans[20];
// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
// Interpolated values from the vertex shaders
in vec2 UV;
uniform float entlight;
uniform vec2 rnd;
out vec4 color;
in float lightamts[20];

float random( vec2 p )
{
  // We need irrationals for pseudo randomness.
  // Most (all?) known transcendental numbers will (generally) work.
  const vec2 r = vec2(
    23.1406926327792690,  // e^pi (Gelfond's constant)
     2.6651441426902251); // 2^sqrt(2) (Gelfond–Schneider constant)
  return fract( cos( mod( 123456789., 1e-7 + 256. * dot(p,r) ) ) );  
}

void main(){
	float lights[20];
	vec3 lightcolor = vec3(0, 0, 0);
	float lighting = 0;
	if(texture( myTextureSampler, UV ).a == 0.0)
		discard;
	for(int i = 0;i < numlights;i++){
		lights[i]=max(0, lightstrengths[i] * exp(-(distance(vec3(gl_FragCoord.xy, 0), lightpositions[i]) + lightspans[i])/lightspans[i]));
		//If it's close enough to the maximum brightness, make it the light's full brightness
		if(lights[i] >= lightstrengths[i] * .27)
			lights[i] += lightstrengths[i] / 8;
		if(lights[i] >= lightstrengths[i] * .22)
			lights[i] += lightstrengths[i] /10;
			//lights[i] = lights[i] * random(rnd + gl_FragCoord.xy);
		lighting+=lights[i];
	}
	//Go through each light and figure out how much of the color it controls
	for(int i = 0;i < numlights;i++){
		lightcolor+=lightcolors[i] * lights[i] / lighting;
	}
	// Output color = color of the texture at the specified UV
	if(entlight != 0)
		color.rgb = texture(myTextureSampler, UV ).rgb * max((1.0 - shadow), entlight);
	else
		color.rgb = mix(texture(myTextureSampler, UV ).rgb, lightcolor, lighting) * max((1.0 - shadow), lighting);
	color.a = texture( myTextureSampler, UV ).a - transparency;
}