// -------------------------------------------------------------------------------------------------------------------------------------------------------------
// - MAIN FUNCTION
// -------------------------------------------------------------------------------------------------------------------------------------------------------------
void main(void) {
	// - gl_FragData[0]
	//	.x : Visibility Buffer (TSM)
	// 	.y : Distance entre les texels receivers (current et previous frames)
	//	.z : Coefficient de condifence
	//	.w : ÃÂÃÂ  dÃÂÃÂ©finir
	gl_FragData[0] = vec4( 1.0, 0.0, 0.0, 0.0 );

	// - gl_FragData[1]: Positions des Texels Receiver
	gl_FragData[1] = vec4(0.);

	// - gl_FragData[2]: Positions des Texels Occluders
	gl_FragData[2] = vec4(0.);
}

