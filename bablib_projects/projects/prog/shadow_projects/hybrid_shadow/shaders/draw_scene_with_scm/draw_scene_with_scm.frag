// Define used GLSL version
#version 140
//#extension GL_EXT_gpu_shader4 : enable
// http://www.opengl.org/registry/specs/EXT/geometry_shader4.txt
//  The built-in special variable gl_PrimitiveIDIn is not an array and has no
//    vertex shader equivalent. It is filled with the number of primitives
//    processed since the last time Begin was called (directly or indirectly via
//    vertex array functions).  The first primitive generated after a Begin is
//    numbered zero, and the primitive ID counter is incremented after every
//    individual point, line, or triangle primitive is processed.  For triangles
//    drawn in point or line mode, the primitive ID counter is incremented only
//    once, even though multiple points or lines may be drawn. Restarting a
//    primitive topology using the primitive restart index has no effect on the
//    primitive ID counter.

uniform sampler2D tex_scm;

uniform vec2 	v2_scm_size;
uniform vec2	v2_screen_size;

//#include "../outils/outils_shader.frag"

#define RENDER_SHADOW
//
#define __USE_SMOOTH_STEP_SHADOW__
float f_coef_min_clamp = 0.0;
float f_coef_max_clamp = 1.0;
//
#define __USE_AMBIANT_SHADOW__
float f_coef_ambiant_shadow = 0.05;

//#define RENDER_COLOR

	//#define __USE_FILTER_COLOR

// -------------------------------------
// - SUBPIXEL - Tests
// - liÃÂÃÂ© directement avec [vbo_scm.frag]
// -------------------------------------
//#define __USE_EXPERIMENTAL_SUBPIXEL_SHADOWED
// -------------------------------------

#define GREEN	vec4( 0, 1, 0, 0 )
#define RED	vec4( 1, 0, 0, 0 )
#define BLUE	vec4( 0, 0, 1, 0 )
//
#define	WHITE	vec4( 1 )
#define BLACK	vec4( 0 )
//
#define CYAN	GREEN + BLUE
#define PURPLE	RED + BLUE
#define YELLOW	RED + GREEN

float 	coef_iso_color = 0.5;

flat in vec4 object_color;

vec4 	interpolate_bicubic_fast(sampler2D tex, vec2 _texCoord, vec2 _sizeTexture);
vec4 	filterColor( vec4 out_color );
//
vec4 	compute_subpixel_tests( in vec4 _inputs );
vec4 	compute_subpixel_test1( in vec4 _inputs );
vec4 	compute_subpixel_test2( in vec4 _inputs );
vec4 	compute_subpixel_test3( in vec4 _inputs );

void main(void) {
	vec2 v2TexelPosition 	= gl_FragCoord.xy / v2_screen_size;
	
	vec4 texSample_bc 	= interpolate_bicubic_fast(tex_scm, v2TexelPosition, v2_scm_size );
	vec4 texSample_bl 	= texture2D(tex_scm, v2TexelPosition );
	
	//vec4 texSample = abs( texSample_bl - texSample_bc );
	//vec4 texSample = texSample_bc;
	//vec4 texSample = texSample_bl.x > 0.5 ? vec4(1.) : vec4(0.);
	vec4 texSample = texSample_bl;
	
	vec4 out_color = vec4(1.);

	#ifdef RENDER_COLOR
		out_color *= object_color;
	#endif

	#ifdef RENDER_SHADOW
		float f_coef_shadow = texSample.r;
		float f_coef_lighting = texSample.w;
		
		f_coef_shadow *= f_coef_lighting;

		#ifdef __USE_SMOOTH_STEP_SHADOW__
			f_coef_shadow = smoothstep( f_coef_min_clamp, f_coef_max_clamp, f_coef_shadow);
		#endif

		#ifdef __USE_AMBIANT_SHADOW__
			f_coef_shadow = max(f_coef_shadow, f_coef_ambiant_shadow);
		#endif
		
		//f_coef_shadow = texSample.z < 5. ? 0.0 : f_coef_shadow;
		out_color *=  f_coef_shadow;
	#endif
	
	#ifdef __USE_EXPERIMENTAL_SUBPIXEL_SHADOWED
		out_color = compute_subpixel_tests( texSample );
	#endif

	//out_color = texSample.y * vec4(1) * 100;					// draw distance entre lex texels receiver (current previous frames)
	//out_color = texSample.w == 1.0 ? vec4(0, 1, 0, 0) : vec4(1);				// draw subpixel_shadowed
	//out_color = (texSample.z <= (0.18*0.18)) ?  vec4(0, 1, 0, 0) * out_color : out_color;	// draw distance receiver occluder
	//out_color = (texSample.z - 1.0) * vec4(1);
//	out_color = vec4(texSample.z) / 25.;

	//out_color = vec4(texSample.z / 25.0);
	//vec4 out_color = gl_Color * texSample;
	//vec4 out_color = output_color * texSample;
//	out_color = vec4(texSample.w);

	#ifdef __USE_FILTER_COLOR
		out_color = filterColor( out_color );
	#endif

	gl_FragColor = out_color;
	//gl_FragColor = vec4( v2TexelPosition.x, v2TexelPosition.y, 0, 1);
}

vec4 compute_subpixel_tests( in vec4 _inputs )
{
	vec4	out_color;

	float	pixels_RO_are_closer		= _inputs.x,
		pixel_is_shadowed_by_plane 	= _inputs.y,
		pixel_shadowed_by_texel_sm 	= _inputs.z,
		subpixel_proj_in_triangle 	= _inputs.w;
	
//	out_color = compute_subpixel_test1( _inputs );
//	out_color = compute_subpixel_test2( _inputs );
	out_color = compute_subpixel_test3( _inputs );
	
	return out_color;
}

vec4 compute_subpixel_test1(in vec4 _inputs)
{
	vec4	out_color;

	float	pixels_RO_are_closer		= _inputs.x,
		pixel_is_shadowed_by_plane 	= _inputs.y,
		pixel_shadowed_by_texel_sm 	= _inputs.z,
		subpixel_proj_in_triangle 	= _inputs.w;

	if ( bool(pixel_shadowed_by_texel_sm) )
	{
		// + RED
		if ( bool(subpixel_proj_in_triangle) )
		{
			// + GREEN (+ RED) 
			if ( bool(pixel_is_shadowed_by_plane) )
			{
				// + BLUE (+ GRENN + RED) = WHITE
				// => pixel-eyes 	est ombrÃÂÃÂ© par un texel-sm
				// => pixel-projetÃÂÃÂ© 	est inclu la projection du triangle occulteur (sur le near plane de camera_light)
				// => pixel-world	est du cotÃÂÃÂ© "ombrÃÂÃÂ©" du plan support du triangle-occulteur
				// <=> le pixel-world est ombrÃÂÃÂ© par le triangle-occulteur (world space)
				// <=> "ombrÃÂÃÂ©"
				out_color = BLUE + GREEN + RED;
			}
			else
			{
				// (+ GREEN + RED) = YELLOW
				// => pixel-eyes 	est ombrÃÂÃÂ© par un texel-sm
				// => pixel-projetÃÂÃÂ© 	est inclue la projection du triangle occulteur (sur le near plane de camera_light)
				// => pixel-world	est du cotÃÂÃÂ© "non-ombrÃÂÃÂ©" du plan support du triangle 
				// <=> le pixel-world est n'est pas ombrÃÂÃÂ© par le triangle-occulteur (world space)
				// <=> Devrait correspondre au texel-eyes en auto-ombrage erronÃÂÃÂ©
				// <=> ! "non-ombrÃÂÃÂ©" !
				out_color = GREEN + RED;
				out_color *= 0.25;
			}
		}
		else
		{
			// (+ RED )
			if ( bool(pixel_is_shadowed_by_plane) )
			{
				if ( bool(pixels_RO_are_closer) )
				{
					// +BLUE +GREEN = TURQUOISE
					// => pixel-eyes 	est ombrÃÂÃÂ© par un texel-sm
					// => pixel-projetÃÂÃÂ© 	est n'est pas inclu la projection du triangle-occulteur (sur le near plane de camera_light)
					// => pixel-world	est du cote "ombrÃÂÃÂ©" du plan support du triangle-occulteur (associciÃÂÃÂ© au texel-sm)
					// => pixel-eyes	est pas "trop prÃÂÃÂ¨s" du texel-SM
					// <=> On peut le considÃÂÃÂ©rer comme "bad-self-shadow" car il est ombrÃÂÃÂ© (texel-sm, plane support du triangle-occulteur) mais est trÃÂÃÂ¨s prÃÂÃÂ¨s (trop prÃÂÃÂ¨s)
					// <=> du plan-occulteur (i.e triangle-occulteur, micro-quad-occulteur)
					// <=> ? "non-ombrÃÂÃÂ©" ?
					out_color = BLUE + GREEN;
					out_color *= 0.4;
				}
				else
				{
					// + BLUE (+RED) = PURPLE
					// => pixel-eyes 	est ombrÃÂÃÂ© par un texel-sm
					// => pixel-projetÃÂÃÂ© 	est n'est pas inclu la projection du triangle-occulteur (sur le near plane de camera_light)
					// => pixel-world	est du cote "ombrÃÂÃÂ©" du plan support du triangle-occulteur (associciÃÂÃÂ© au texel-sm)
					// => pixel-eyes	n'est pas "trop prÃÂÃÂ¨s" du texel-SM
					// <=> Devrait correspondre au texel-eyes dont on ne dispose pas (encore) d'assez d'information pour ÃÂÃÂ©tablir son status
					// <=> ? "ombrÃÂÃÂ©" ?
					out_color = BLUE + RED;
					out_color *= 0.6;
				}
			}
			else
			{
				// (+RED) = RED
				// => pixel-eyes 	est ombrÃÂÃÂ© par un texel-sm
				// => pixel-projetÃÂÃÂ© 	est n'est pas inclu la projection du triangle-occulteur (sur le near plane de camera_light)
				// => pixel-world	est du cote "non-ombrÃÂÃÂ©" du plan support du triangle-occulteur (associciÃÂÃÂ© au texel-sm)
				// <=> Devrait correspondre au texel-eyes dont on ne dispose pas (encore) d'assez d'information pour ÃÂÃÂ©tablir son status
				// <=> ? "non-ombrÃÂÃÂ©" ?
				out_color = RED;
				out_color *= 0.4;
			}
		}
	}
	else
	{
		// Object_Color grisÃÂÃÂ©
		// => pixel-eyes	n'est pas ombrÃÂÃÂ© par un texel-sm
		// <=> "non-ombrÃÂÃÂ©"
		out_color = object_color * 0.0;
	}

	return out_color;
}

vec4 compute_subpixel_test2(in vec4 _inputs)
{
//			- Est ce qu'il est trÃÂÃÂ¨s proche de son occluder ?
//			* si oui: forte chance de se trouver dans un cas de self-shadowing
//				
//				* si oui:
//					- Est ce qu'il est du cotÃÂÃÂ© ombrÃÂÃÂ© du plan supportant le triangle occulteur ?
//					* si oui: 
//						=> (1) Cas ambigu: 
//							Pixel ombrÃÂÃÂ© correctement thÃÂÃÂ©oriquement
//							En pratique, ce cas n'est pas possible, 
//							car le texel doit ÃÂÃÂªtre trÃÂÃÂ¨s proche du triangle-caster,
//							du cotÃÂÃÂ© ombrÃÂÃÂ© du plan support du triangle-caster,
//							et ÃÂÃÂªtre dans la projection du triangle.
//							Les seuls texels possibles sont les texels inclus dans le volume (d'ombre) "infinitÃÂÃÂ©simale" 
//							de la projection du triangle-caster par rapport ÃÂÃÂ  la source de lumiÃÂÃÂ¨re"
//							Si des texels de ce type apparaissent, ils sont surement du ÃÂÃÂ  une erreur de prÃÂÃÂ©cision
//							=> A priori ces texels doivent ÃÂÃÂªtre non-ombrÃÂÃÂ©s (cas de self-shadowing)
//					* si non: 
//						=> (2) Pixel ombrÃÂÃÂ© incorrectement, il est dans le volume de projection du triangle mais pas dans son volume d'ombre !
//				* si non: 
//						=> (3) Pixel potentiellement non-ombrÃÂÃÂ©, le triangle rasterisÃÂÃÂ© dans le texel-SM ne permet de fixer le statut du texel-receiver
//			* si non: peu de chance de se trouver dans un cas de self-shadowing
//				- Est ce qu'il est dans la projection de ce triangle ?
//				* si oui:
//					- Est ce qu'il est du cotÃÂÃÂ© ombrÃÂÃÂ© du plan supportant le triangle occulteur ?
//					* si oui: 
//						=> (4) Pixel ombrÃÂÃÂ© correctement
//					* si non: 
//						=> (5) Pixel potentiellement ombrÃÂÃÂ©, le triangle rasterisÃÂÃÂ© dans le texel-SM ne permet de fixer le statut du texel-receiver
//				* si non:
//					- Est ce qu'il est du cotÃÂÃÂ© ombrÃÂÃÂ© du plan supportant le triangle occulteur ?
//					* si oui: 
//						=> (6) Pixel potentiellement ombrÃÂÃÂ©, le triangle rasterisÃÂÃÂ© dans le texel-SM ne permet pas de dÃÂÃÂ©finir son statut.	
//					* si non: 
//						=> (7) Pixel potentiellement ombrÃÂÃÂ©, le triangle rasterisÃÂÃÂ© dans le texel-SM ne permet pas de dÃÂÃÂ©finir son statut.
//		si non: 
//			=> (8) ÃÂÃÂ  priori pas de sushis, du moins il n'y a aucune informations (triangle occulteur rasterisÃÂÃÂ©) exploitables pour l'instant

	vec4	out_color;

	float	pixels_RO_are_closer		= _inputs.x,
		pixel_is_shadowed_by_plane 	= _inputs.y,
		pixel_shadowed_by_texel_sm 	= _inputs.z,
		subpixel_proj_in_triangle 	= _inputs.w;

	if ( bool(pixel_shadowed_by_texel_sm) )
	{
		if ( bool(pixels_RO_are_closer) )
		{ // [LIT]
			if ( bool(subpixel_proj_in_triangle) )
			{
				if ( bool(pixel_is_shadowed_by_plane) )
				{
					out_color = RED;		// (1) [LIT]
				}
				else
				{
					out_color = GREEN;		// (2) [LIT]
				}
			}
			else
			{
				out_color = BLUE;			// (3) [LIT]
			}
		}
		else 
		{ // [SHADOWED]
			if ( bool(subpixel_proj_in_triangle) )
			{
				if ( bool(pixel_is_shadowed_by_plane) )
				{
					out_color = YELLOW;		// (4) [SHADOWED]
				}
				else
				{
					out_color = PURPLE;		// (5) [SHADOWED]
				}
			}
			else
			{
				if ( bool(pixel_is_shadowed_by_plane) )
				{
					out_color = CYAN;		// (6) [SHADOWED] 
				}
				else
				{
					out_color = WHITE;		// (7) [SHADOWED]
				}
			}

		}
	}
	else
	{
		out_color = BLACK;					// (8) [LIT]
	}

	return out_color;
}

vec4 compute_subpixel_test3(in vec4 _inputs)
{
//	Ordres de prioritÃÂÃÂ©s:
//		- Est ce que le texel-receiver courant est ombrÃÂÃÂ© ?
//		* si oui:	
//			- Est ce qu'il est dans la projection du triangle-caster ?
//			* si oui: 
//				- Est ce qu'il est du cotÃÂÃÂ© ombrÃÂÃÂ© du plan supportant le triangle-caster ?
//				* si oui:
//					=> Texel ombrÃÂÃÂ© (correctement)
//				* si non:
//					=> Texel ÃÂÃÂ©clairÃÂÃÂ©
//			* si non:
//				- Est ce qu'il est du cotÃÂÃÂ© ombrÃÂÃÂ© du plan supportant le triangle-caster ?
//				* si oui:
//					- Est ce qu'il est trÃÂÃÂ¨s proche de son occluder ?
//					* si oui:
//						=> Texel ÃÂÃÂ©clairÃÂÃÂ©
//					* si non:
//						=> Texel ombrÃÂÃÂ©
//				* si non:
//					=> Texel ÃÂÃÂ©clairÃÂÃÂ©
//		* si non:
//			=> pas du sushis, texel ÃÂÃÂ©clairÃÂÃÂ©

	vec4	out_color;

	bool	pixels_RO_are_closer		= bool(_inputs.x),
		pixel_is_shadowed_by_plane 	= bool(_inputs.y),
		pixel_shadowed_by_texel_sm 	= bool(_inputs.z),
		subpixel_proj_in_triangle 	= bool(_inputs.w);

	float f_coef_shadowed 	= 0.75;
	float f_coef_lit	= 1.0;

	if (pixel_shadowed_by_texel_sm)
	{
		if (subpixel_proj_in_triangle)
		{
			if (pixel_is_shadowed_by_plane)
			{
				out_color = RED;
				out_color *= f_coef_shadowed;
			}
			else
			{
				out_color = GREEN;
				out_color *= f_coef_lit;
			}
		}
		else
		{
			if (pixel_is_shadowed_by_plane)
			{
				if (pixels_RO_are_closer)
				{
					out_color = BLUE;
					out_color *= f_coef_lit;
				}
				else
				{
					out_color = YELLOW;
					out_color *= f_coef_shadowed;
				}
			}
			else
			{
				if (pixels_RO_are_closer)
				{
					out_color = PURPLE;
					out_color *= f_coef_lit;
				}
				else
				{
					out_color = BLACK;
					out_color *= f_coef_shadowed;
				}
			}
		}
	}
	else
	{
		out_color = WHITE;
		out_color *= f_coef_lit;
	}

	return out_color;
}
// Texture 2D
vec4 interpolate_bicubic_fast(sampler2D tex, vec2 _texCoord, vec2 _sizeTexture)
{
	float 	x = _texCoord.x * _sizeTexture.x,
		y = _texCoord.y * _sizeTexture.y;
	 
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
	h0 /= _sizeTexture;
	h1 /= _sizeTexture;
	
	// fetch the four linear interpolations
	vec4 tex00 = texture2D(tex, vec2(h0.x, h0.y) );
	vec4 tex10 = texture2D(tex, vec2(h1.x, h0.y) );
	vec4 tex01 = texture2D(tex, vec2(h0.x, h1.y) );
	vec4 tex11 = texture2D(tex, vec2(h1.x, h1.y) );

	// weigh along the y-direction
	tex00 = mix(tex01, tex00, g0.y);
	tex10 = mix(tex11, tex10, g0.y);

	// weigh along the x-direction
	return mix(tex10, tex00, g0.x);
}

vec4 filterColor( vec4 out_color )
{
	// exhibe 3 frontiÃÂÃÂ¨res (3 courbes iso)
	/**/
	vec4 out_color_iso0 	= abs( out_color.x - 0.05) <= 0.04 ? vec4(abs( out_color.x - 0.05) / 0.04, 0, 0, 0) : vec4(0.0);
	vec4 out_color_iso1 	= abs( out_color.x - 0.95) <= 0.04 ? vec4( 0, abs(out_color.x - 0.95) / 0.04, 0, 0) : vec4(0.0);
	vec4 out_color_iso05 	= abs( out_color.x - 0.5) <= 0.04 ? vec4( 0, 0, abs(out_color.x - 0.5) / 0.04, 0) : vec4(0.0);
	//
	vec4 iso_color = out_color_iso0 + out_color_iso1 + out_color_iso05;
	iso_color *= coef_iso_color;
	out_color = length(iso_color) > 0.0 ? iso_color : out_color;
	/**/
	
	/**
	vec4 out_color_iso05 = smoothstep( vec4(0.5-0.04), vec4(0.5+0.04), out_color ) * vec4(0, 0, 1, 0);
	out_color = mix( out_color_iso05, out_color, abs( out_color.x - 0.5) > 0.04);
	/**/
	
	/**
	float coef_clamp = 0.5;
	out_color = step( coef_clamp, out_color );
	/**/
	
	return out_color;
	}
