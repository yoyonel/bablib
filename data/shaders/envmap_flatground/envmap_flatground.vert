varying vec3 worldViewDir;
varying vec3 p0;

void main() {
    gl_Position = 2 * gl_Vertex - 1;
    vec4 e = gl_ModelViewProjectionMatrixInverse * vec4(0,0,-1,0);
    vec4 p = gl_ModelViewProjectionMatrixInverse * gl_Position;
    p0 = e.xyz / e.w;
    //p0 = p.xyz / p.w;
    worldViewDir = sign(e.w) * sign(p.w) * (e.w * p.xyz - p.w * e.xyz);
    }

