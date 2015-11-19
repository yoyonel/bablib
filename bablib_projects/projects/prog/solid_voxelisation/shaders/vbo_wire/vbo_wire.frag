#define GREEN		vec4( 0, 1, 0, 0 )
#define RED		vec4( 1, 0, 0, 0 )
#define BLUE		vec4( 0, 0, 1, 0 )
//
#define EPSILON		(0.000001)		//@@ trouver l'epsilon des floats (autour de 0.)

#define COMPUTE_LIGHTING( v3_vertex, v3_normal, v3_light_pos ) dot( normalize(v3_light_pos - v3_vertex), v3_normal );

//#define __USE_DIFFUSE_COLOR__
#define __USE_DIFFUSE_LIGHTING__
// PHONG diffuse shading
//#define __USER_PER_PIXEL_LIGHTING__
// GOURAUD diffuse shading
#define __USER_PER_VERTEX_LIGHTING__
//#define USE_AMBIENT_LIGHTING
//
#define __RENDER_ANALOGIC_EDGE__

//#define __USE_AREA_LIGHTING__

uniform float 	u_f_coef_ambient_lighting;
uniform vec3 	u_v3_light_pos_in_object;

in vec4     v_v4_color;
in vec3     v_v3_position;
in vec3     v_v3_normal;
in float    v_f_diffuse_lighting;

void    compute_edge(in float f_coef_lighting, out bool b_is_edge, out float f_coef_edge);
float   compute_area_lighting();

void main()
{
    float f_diffuse_pp_lighting, f_diffuse_pv_lighting;

    vec4 out_color = vec4(1.);

#ifdef __USE_DIFFUSE_COLOR__
    out_color = v_v4_color;
#endif

#ifdef __USE_DIFFUSE_LIGHTING__
    float f_coef_lighting = 1.0;

    // PHONG shading
    f_diffuse_pp_lighting = COMPUTE_LIGHTING( v_v3_position, v_v3_normal, u_v3_light_pos_in_object );

    // GOURAUD shading
    f_diffuse_pv_lighting = v_f_diffuse_lighting;

#ifdef __USER_PER_PIXEL_LIGHTING__
    f_coef_lighting = f_diffuse_pp_lighting;
#endif

#ifdef __USER_PER_VERTEX_LIGHTING__
    f_coef_lighting = f_diffuse_pv_lighting;
#endif

#ifdef USE_AMBIENT_LIGHTING
    f_coef_lighting = min(1., f_coef_lighting + u_f_coef_ambient_lighting);
#endif

    //out_color *= f_coef_lighting;
    out_color *= f_coef_lighting < 0.0 ? (f_coef_lighting > -0.1 ? RED : RED+BLUE) : (f_coef_lighting < 0.1 ? GREEN : GREEN+BLUE);
#endif

    gl_FragData[0] = out_color;
}
