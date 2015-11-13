uniform samplerCube cubeMap;
varying vec3 worldViewDir;
uniform float brightness = 1.0;

void main() {
    //gl_FragColor = brightness * textureCube(cubeMap, worldViewDir);
    gl_FragColor = brightness * textureCube(cubeMap, worldViewDir.yzx);
    //if (worldViewDir.z < 0) gl_FragColor = vec4(1,0,0,1);
    }

