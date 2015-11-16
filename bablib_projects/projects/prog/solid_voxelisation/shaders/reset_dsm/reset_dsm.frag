#define V4_ZERO	vec4(0.);

void main(void) {
	gl_FragDepth = 0.;
	
	// MRT
	// Positions in light space or Projective Light Space
	// - FragData[0]: Vertex0 du Triangle
	// - FragData[1]: Vertex1 du Triangle
	// - FragData[2]: Vertex2 du Triangle
	gl_FragData[0] = V4_ZERO;
	gl_FragData[1] = V4_ZERO;
	gl_FragData[2] = V4_ZERO;

	//
	gl_FragData[3] = V4_ZERO;
}

