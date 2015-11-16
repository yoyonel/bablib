// Define used GLSL version
#version 120

uniform sampler2D texture;

uniform float tex_width, tex_height;

void main(void) {
    vec4 texSample = texture2D(texture, gl_TexCoord[0].xy);
    gl_FragColor= texSample;
}
