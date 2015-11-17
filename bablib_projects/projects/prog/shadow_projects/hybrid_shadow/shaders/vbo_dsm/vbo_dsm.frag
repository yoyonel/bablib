// Define used GLSL version
#version 120

varying float 	v_f_is_triangle_silhouette;
varying float 	v_f_coef_lighting;

void main(void) {
	//gl_FragDepth = gl_FragCoord.z;
	
	// MRT
	// - FragData[0]: Vertex0 du Triangle
        gl_FragData[0] = vec4(v_f_is_triangle_silhouette);
//	gl_FragData[0] = vec4(-v_f_coef_lighting);
}
