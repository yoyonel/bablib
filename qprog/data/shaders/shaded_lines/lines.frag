varying vec3 viewDir;
varying vec3 normal;
varying vec4 color;

void main() {
    //gl_FragColor = vec4(dot(vec3(0,1,0), normal) * vec3(0, 0.5f, 0), 1);
    float c = -dot(normalize(viewDir), normalize(normal));
    float s = sqrt(1 - c*c);
    gl_FragColor = vec4(s * color.rgb, color.a);
    //gl_FragColor = vec4(color.rgb, color.a);
    }
