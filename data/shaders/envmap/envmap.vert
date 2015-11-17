varying vec3 worldViewDir;

void main() {
    gl_Position = 2 * gl_Vertex - 1;
    vec4 e = gl_ModelViewProjectionMatrixInverse * vec4(0,0,-1,0);
    vec4 p = gl_ModelViewProjectionMatrixInverse * gl_Position;
    worldViewDir = sign(e.w) * sign(p.w) * (e.w * p.xyz - p.w * e.xyz);
    }

