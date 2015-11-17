// Define used GLSL version
#version 120

uniform sampler1D bitmask;

varying float depth;

void main() {
    if ( depth < -0.001 )
	gl_FragColor = vec4( 1, 1, 0, 0);
    else if ( depth > 1.01 )
	gl_FragColor = vec4( 1, 0, 1, 0);
    else
	gl_FragColor = texture1D( bitmask, depth );
	//gl_FragColor = vec4( 0, 0, 0, 0);

    //gl_FragColor = vec4( depth );
    //gl_FragColor = vec4( 1, 0, 0, 0);
}

