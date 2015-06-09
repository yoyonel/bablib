varying vec3 worldViewDir;
varying vec3 p0;

/*
varying vec3 dx;
varying vec3 dy;
*/
varying vec3 p1;
varying vec3 dx0;
varying vec3 dx1;
varying vec3 dy0;
varying vec3 dy1;
//@ a voir : redondants si pixels carres : dans ce cas juste deux float a passer..

uniform ivec2 res;

void main() {
    //gl_Position = 2 * gl_Vertex - 1;
    //vec2 p = gl_Position.xy;
    
    vec2 p = 2 * gl_Vertex.xy - 1;
    gl_Position = vec4(p, 0, 1);
    
    /*
    vec4 e = gl_ModelViewProjectionMatrixInverse * vec4(0,0,-1,0);
    vec4 p = gl_ModelViewProjectionMatrixInverse * gl_Position;
    p0 = e.xyz / e.w;
    //p0 = p.xyz / p.w;
    worldViewDir = sign(e.w) * sign(p.w) * (e.w * p.xyz - p.w * e.xyz);
    */

    vec4 P0 = gl_ModelViewProjectionMatrixInverse * vec4(p, -1, 1);
    vec4 P1 = P0 + 2 * gl_ModelViewProjectionMatrixInverse[2];  // == T * vec4(p, 1, 1);
    p0 = P0.xyz / P0.w;
    p1 = P1.xyz / P1.w;
    worldViewDir = p1 - p0;
    //worldViewDir = sign(P0.w) * sign(P1.w) * (P0.w * P1.xyz - P1.w * P0.xyz);
    
    vec2 dpix = 1.0 / res;
    vec4 Dx = dpix.x * gl_ModelViewProjectionMatrixInverse[0];      // == T * (dpix.x, 0, 0, 0)
    vec4 Dy = dpix.y * gl_ModelViewProjectionMatrixInverse[1];      // == T * (0, dpix.y, 0, 0)
    
    vec4 px0 = P0 + Dx;   // == T * vec4(p + vec2(dpix.x, 0), -1, 1)
    vec4 px1 = P1 + Dx;   // == T * vec4(p + vec2(dpix.x, 0),  1, 1)
    vec4 py0 = P0 + Dy;   // == T * vec4(p + vec2(0, dpix.y), -1, 1)
    vec4 py1 = P1 + Dy;   // == T * vec4(p + vec2(0, dpix.y),  1, 1)

    dx0 = px0.xyz / px0.w - p0;
    dx1 = px1.xyz / px1.w - p1;
    dy0 = py0.xyz / py0.w - p0;
    dy1 = py1.xyz / py1.w - p1;
    }

