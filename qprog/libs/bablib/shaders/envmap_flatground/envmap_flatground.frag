varying vec3 worldViewDir;
varying vec3 p0;                // world coords of view point
uniform samplerCube cubeMap;
uniform float brightness = 1.0;
uniform vec3  projCenter = vec3(0,0,1);     // center of projection of the cube-map on the ground (theoretically = point from which the cube-map was sampled)
uniform float projRadius = 1.0;             // distance from projCenter to the ground

const vec4 colorHaze = vec4(0.25, 0.35, 0.45, 1);
vec4 envColor(const in vec3 d) { return brightness * textureCube(cubeMap, d.yzx); }

void main() {
    if (worldViewDir.z < 0) {   // mapping of the cube-map on the plane z=0
        vec3 q = p0 - (p0.z / worldViewDir.z) * worldViewDir;   // point on the plane z=0
        if (p0.z < 0 || all(lessThan(abs(q), projRadius))) discard;
        
        vec3 d = q - projCenter;
        
        float a = max(0.0, length(q.xy) - sqrt(2.0) * projRadius);
        gl_FragColor = mix(colorHaze, envColor(d), exp(-a / (10 * projRadius)));
        }
    else {
        float horiz = worldViewDir.z / length(worldViewDir);
        float horizMin = 0.015;
        gl_FragColor = mix(colorHaze, envColor(worldViewDir), clamp(horiz / horizMin, 0.0, 1.0));
        }
    }

