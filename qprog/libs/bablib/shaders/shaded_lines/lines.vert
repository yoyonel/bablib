varying vec3 viewDir;
varying vec3 normal;
varying vec4 color;

void main() {
    //pos = gl_MultiTexCoord0.xy;
    //pos = gl_Vertex.xy;
    color = gl_Color;
    //gl_Position = gl_ModelViewProjectionMatrix * vec4(center + scale * (pos - 0.5f), 0, 1);

    normal = gl_Normal.xyz;
    
    vec4 eye = gl_ModelViewProjectionMatrixInverse * vec4(0,0,-1,0);    // coordonnées de l'oeil dans le repère local
    viewDir = (eye.w * gl_Vertex.xyz - eye.xyz);    //@ * sign(eye.w) ...
    gl_Position = ftransform();
    }
