// Define used GLSL version
#version 120

// Uniform parameters (=global)
uniform mat4 u_m4_mv_light;		// Uniform Mat4 ModelView to Light (space)
uniform mat4 u_m4_modelview;		// Uniform Mat4 ModelView (Object Space -> World Space)
//
uniform vec4 u_v4_mv_light_pos;		// Uniform Vec4 ModelView Light Position
//
uniform mat4 u_m4_mvp_prev;
//
varying vec4 texelLightSpace;
varying vec4 texelWorldSpace;
//
varying vec4 position_prev;
//
varying vec3 normal;
varying vec4 color;
varying vec3 lightDir;


void main(void) {
	gl_Position = ftransform();

	texelLightSpace = u_m4_mv_light * gl_Vertex;
	texelWorldSpace = u_m4_modelview * gl_Vertex;
	position_prev 	= u_m4_mvp_prev * texelWorldSpace;

    #define WORLD_SPACE_LIGHTING
    #ifdef WORLD_SPACE_LIGHTING
        normal = gl_Normal;
        //vec4 L = gl_ModelViewProjectionMatrixInverse * vec4(0,0,-1,0);
	vec4 L = u_v4_mv_light_pos;
        //lightDir = L.xyz;
        lightDir = L.xyz / L.w - gl_Vertex.xyz / gl_Vertex.w;
    #else
        normal = gl_NormalMatrix * gl_Normal;
        lightDir = vec3(0,0,1);
    #endif
}