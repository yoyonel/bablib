// -------------------------------------------------------------------------------------------------------------------------------------------------------------
// MACROS
// -------------------------------------------------------------------------------------------------------------------------------------------------------------
#define COMPUTE_LIGHTING( v3_vertex, v3_normal, v3_light_pos ) dot( normalize(v3_light_pos - v3_vertex), v3_normal );
// -------------------------------------------------------------------------------------------------------------------------------------------------------------

uniform vec3 	u_v3_light_pos_in_object;

varying vec4	v_v4_color;
varying	vec3	v_v3_position;
varying	vec3	v_v3_normal;
varying	vec3	v_v3_normalEyeSpace;
varying float	v_f_diffuse_lighting;

void main(void) {
	gl_Position = ftransform();

	v_v4_color = gl_Color;

	v_v3_position 	= gl_Vertex.xyz;
	v_v3_normal	= gl_Normal.xyz;
        // url: http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
        v_v3_normalEyeSpace = vec3(gl_ModelViewMatrix * vec4(gl_Normal,0.0));

	v_f_diffuse_lighting = COMPUTE_LIGHTING( gl_Vertex.xyz, gl_Normal, u_v3_light_pos_in_object );
}


