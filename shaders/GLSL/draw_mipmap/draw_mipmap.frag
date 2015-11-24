//# ------------------------------------------
//# version 140 => sampler2DRect
//# version > 120 => plus de texCoord
//# ------------------------------------------

#version 120

uniform sampler2D tex_src;

uniform int i_mode_interpolation; // 0 = bilinear, 1 = bicubic
uniform float coef_clamp;
uniform bool b_clamp_actif;

uniform float tex_width, tex_height;

uniform float coefLod;

vec4 interpolate_bicubic_fast(sampler2D tex, vec3 texCoord, vec2 sizeTex);
vec4 interpolate_tricubic_fast(sampler2D tex, vec3 texCoord, vec2 sizeTex);

vec4 filterColor( vec4 out_color );

#define GREEN	vec4( 0, 1, 0, 0 )
#define RED	vec4( 1, 0, 0, 0 )
#define BLUE	vec4( 0, 0, 1, 0 )

#define EPSILON		(0.000001)		//@@ trouver l'epsilon des floats (autour de 0.)

// - Intensité des isos couleurs
float coef_iso_color = 0.1;

// @faire: réfléchir sur l'interpolation tricubic, et particulièrement interprétation du Z par rapport au niveau de Lod (MipMap)
void main(void) {
	vec4 out_color;
	
	//float coefLod = 1.9; // entre [0.0, max_level_mipmap [
	
	vec3 texelCoord =  vec3(gl_TexCoord[0].s, gl_TexCoord[0].t, coefLod);
	vec2 sizeTex = vec2(tex_width, tex_height);
	
	// -- TEXTURE 2D
	vec4 frag_bilinear = texture2D(tex_src, gl_TexCoord[0].st);
	//
	vec4 frag_bilinear_lod = texture2DLod( tex_src, texelCoord.xy, texelCoord.z );
	//
	vec4 frag_bicubic_lod = interpolate_bicubic_fast( tex_src, texelCoord, sizeTex );
	//
	vec4 frag_tricubic_lod = interpolate_tricubic_fast( tex_src, texelCoord, sizeTex );
	
	// - Resultat (color final)
	out_color = frag_tricubic_lod;
	//out_color = frag_bicubic_lod;
	//out_color = frag_bilinear_lod;
	//
	//out_color = abs( frag_bicubic_lod - frag_bilinear_lod) ;
	//out_color = abs( frag_bicubic_lod - frag_tricubic_lod );
	//out_color = abs( frag_tricubic_lod - frag_bilinear_lod );
	
	// -- COLOR FILTERS (affichage des contours (isos courbes))
	//out_color = mix( out_color, filterColor( out_color ), coef_iso_color );
	
	// return result
	gl_FragColor = out_color;
}

// Texture 2D
vec4 interpolate_bicubic_fast(sampler2D tex, vec3 texCoord, vec2 sizeTex)
{
/**/
	// - 
	float x = texCoord.x * sizeTex.x;
	float y = texCoord.y * sizeTex.y;
	 
	// transform the coordinate from [0,extent] to [-0.5, extent-0.5]
	vec2 coord_grid = vec2(x - 0.5, y - 0.5);
	vec2 index 	= floor(coord_grid); 		// nearest integer
	vec2 fraction 	= coord_grid - index;		//
	vec2 one_frac 	= 1.0 - fraction;
	
	// bspline_weights(fraction, w0, w1, w2, w3);
	vec2 w0 = 1.0/6.0 * one_frac*one_frac*one_frac;
	vec2 w1 = 2.0/3.0 - 0.5 * fraction*fraction*(2.0-fraction);
	vec2 w2 = 2.0/3.0 - 0.5 * one_frac*one_frac*(2.0-one_frac);
	vec2 w3 = 1.0/6.0 * fraction*fraction*fraction;

	vec2 g0 = w0 + w1;
	vec2 g1 = w2 + w3;
	vec2 h0 = (w1 / g0) - vec2(0.5) + index;  //h0 = w1/g0 - 1, move from [-0.5, extent-0.5] to [0, extent]
	vec2 h1 = (w3 / g1) + vec2(1.5) + index;  //h1 = w3/g1 + 1, move from [-0.5, extent-0.5] to [0, extent]

	//
	h0 /= sizeTex;
	h1 /= sizeTex;
	
	// fetch the four linear interpolations
	vec4 tex00 = texture2DLod(tex, vec2(h0.x, h0.y), texCoord.z );
	vec4 tex10 = texture2DLod(tex, vec2(h1.x, h0.y), texCoord.z );
	vec4 tex01 = texture2DLod(tex, vec2(h0.x, h1.y), texCoord.z );
	vec4 tex11 = texture2DLod(tex, vec2(h1.x, h1.y), texCoord.z );

	// weigh along the y-direction
	tex00 = mix(tex01, tex00, g0.y);
	tex10 = mix(tex11, tex10, g0.y);

	// weigh along the x-direction
	return mix(tex10, tex00, g0.x);
	//return tex00;
	/**/
}

//! Tricubic interpolated texture lookup, using unnormalized coordinates.
//! Fast implementation, using 8 trilinear lookups.
//! @param tex  	2D textureLod
//! @param texCoord  	unnormalized 2D texture coordinate (.xy), normalized z coord (in lod-texture space)
vec4 interpolate_tricubic_fast(sampler2D tex, vec3 texCoord, vec2 sizeTex)
{
	vec3 coord = vec3( texCoord.xy*sizeTex, texCoord.z );
	
	// shift the coordinate from [0,extent] to [-0.5, extent-0.5]
	vec3 coord_grid = coord - 0.5;
	vec3 index = floor(coord_grid);
	vec3 fraction = coord_grid - index;
	vec3 one_frac = 1.0 - fraction;
	
	// bspline_weights(fraction, w0, w1, w2, w3);
	vec3 w0 = 1.0/6.0 * one_frac*one_frac*one_frac;
	vec3 w1 = 2.0/3.0 - 0.5 * fraction*fraction*(2.0-fraction);
	vec3 w2 = 2.0/3.0 - 0.5 * one_frac*one_frac*(2.0-one_frac);
	vec3 w3 = 1.0/6.0 * fraction*fraction*fraction;

	vec3 g0 = w0 + w1;
	vec3 g1 = w2 + w3;
	vec3 h0 = (w1 / g0) - 0.5 + index;  //h0 = w1/g0 - 1, move from [-0.5, extent-0.5] to [0, extent]
	vec3 h1 = (w3 / g1) + 1.5 + index;  //h1 = w3/g1 + 1, move from [-0.5, extent-0.5] to [0, extent]

	//
	h0 = vec3( h0.xy / sizeTex, h0.z );
	h1 = vec3( h1.xy / sizeTex, h1.z );
	
	// fetch the eight linear interpolations
	// weighting and fetching is interleaved for performance and stability reasons
	vec4 tex000 = texture2DLod(tex, vec2(h0.x, h0.y), h0.z);
	vec4 tex100 = texture2DLod(tex, vec2(h1.x, h0.y), h0.z);
	tex000 = mix(tex100, tex000, g0.x);  //weigh along the x-direction
	vec4 tex010 = texture2DLod(tex, vec2(h0.x, h1.y), h0.z);
	vec4 tex110 = texture2DLod(tex, vec2(h1.x, h1.y), h0.z);
	tex010 = mix(tex110, tex010, g0.x);  //weigh along the x-direction
	tex000 = mix(tex010, tex000, g0.y);  //weigh along the y-direction
	vec4 tex001 = texture2DLod(tex, vec2(h0.x, h0.y), h1.z);
	vec4 tex101 = texture2DLod(tex, vec2(h1.x, h0.y), h1.z);
	tex001 = mix(tex101, tex001, g0.x);  //weigh along the x-direction
	vec4 tex011 = texture2DLod(tex, vec2(h0.x, h1.y), h1.z);
	vec4 tex111 = texture2DLod(tex, vec2(h1.x, h1.y), h1.z);
	tex011 = mix(tex111, tex011, g0.x);  //weigh along the x-direction
	tex001 = mix(tex011, tex001, g0.y);  //weigh along the y-direction

	return mix(tex001, tex000, g0.z);  //weigh along the z-direction
}

vec4 filterColor( vec4 out_color )
{
	vec4 iso_color;
	
	// exhibe 3 frontières (3 courbes iso)
	/**/
	//const float fCoef_Width = 0.5;
	const float fCoef_Width = 0.1;
	vec3 v3CoefISOs;
 	float fCoefIntensity = length( out_color.xyz ) / sqrt(3.);
	//
	v3CoefISOs.x = smoothstep( 0. + EPSILON, 0. + fCoef_Width, fCoefIntensity );
	v3CoefISOs.y = smoothstep( 1. - fCoef_Width, 1.0 - (0.04), fCoefIntensity );
	v3CoefISOs.z = smoothstep( .5 - (fCoef_Width/1.), .5 + (fCoef_Width/1.), fCoefIntensity );
	//
	v3CoefISOs = vec3(1.) - abs((v3CoefISOs*vec3(2.)) - vec3(1.));
	//
	iso_color += v3CoefISOs.x * RED;
	iso_color += v3CoefISOs.y * GREEN;
	iso_color += v3CoefISOs.z * BLUE;
	//
	iso_color *= coef_iso_color;
	//
	
	out_color = length(iso_color) > 0.0 ? iso_color : out_color;
	/**/
	
	/**
	vec4 out_color_iso05 = smoothstep( vec4(0.5-0.04), vec4(0.5+0.04), out_color ) * vec4(0, 0, 1, 0);
	out_color = mix( out_color_iso05, out_color, abs( fCoefIntensity - 0.5) > 0.04);
	/**/
	
	/**
	float coef_clamp = 0.5;
	out_color = step( coef_clamp, out_color );
	/**/
	
	return out_color;
	}
