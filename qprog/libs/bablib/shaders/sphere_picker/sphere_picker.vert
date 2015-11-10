varying vec3 dirRay;
varying vec3 origRay;

void main() {
    vec2 p = 2 * gl_Vertex.xy - 1;
    gl_Position = vec4(p, 0, 1);
    
    vec4 P0 = gl_ModelViewProjectionMatrixInverse * vec4(p, -1, 1);
    vec4 P1 = P0 + 2 * gl_ModelViewProjectionMatrixInverse[2];  // == T * vec4(p, 1, 1);
    vec3 p0 = P0.xyz / P0.w;
    vec3 p1 = P1.xyz / P1.w;
    origRay = p0;
    dirRay = p1 - p0;
    //worldViewDir = sign(P0.w) * sign(P1.w) * (P0.w * P1.xyz - P1.w * P0.xyz);
    
    }

