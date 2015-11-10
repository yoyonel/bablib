varying vec4 color;
//varying vec3 normal;
//varying vec3 lightDir;
void main() {
    //const float d = max(0.0, dot(vec3(0,0,1), normal));
    //const float d = max(0.0, dot(normalize(lightDir), normal));
    //gl_FragColor = vec4(d * color.rgb, color.a);
    gl_FragColor = color;
    }
