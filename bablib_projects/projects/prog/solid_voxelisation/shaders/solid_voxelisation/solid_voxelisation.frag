// Define used GLSL version
#version 120

uniform sampler1D bitmask;

void main() {
    gl_FragColor = texture1D( bitmask, gl_FragCoord.z );
}
