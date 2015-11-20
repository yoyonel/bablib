// -------------------------------------------------------------------------------------------------------------------------------------------------------------
// MACROS
// -------------------------------------------------------------------------------------------------------------------------------------------------------------
#define COMPUTE_LIGHTING( v3_vertex, v3_normal, v3_light_pos ) dot( normalize(v3_light_pos - v3_vertex), v3_normal );
// -------------------------------------------------------------------------------------------------------------------------------------------------------------

uniform vec3 	u_v3_light_pos_in_object;

varying vec4	v_v4_color;
varying	vec3	v_v3_position;
varying	vec3	v_v3_normal;
varying float	v_f_diffuse_lighting;

void main(void) {
	gl_Position = ftransform();

	v_v4_color = gl_Color;

	v_v3_position 	= gl_Vertex.xyz;
	v_v3_normal	= gl_Normal.xyz;

	v_f_diffuse_lighting = COMPUTE_LIGHTING( gl_Vertex.xyz, gl_Normal, u_v3_light_pos_in_object );
}

