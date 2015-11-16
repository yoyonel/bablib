// Define used GLSL version
#version 120

uniform sampler1D bitmask;
varying float depth;

void main() {
    gl_FragColor = texture1D( bitmask, depth );
}

