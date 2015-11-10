varying vec4 color;
//varying vec3 normal;
//varying vec3 lightDir;

void main() {
    gl_Position = ftransform();
    //color = gl_Color;
    
    //#define WORLD_SPACE_LIGHTING
    #ifdef WORLD_SPACE_LIGHTING
        vec3 normal = gl_Normal;
        vec4 L = gl_ModelViewProjectionMatrixInverse * vec4(0,0,-1,0);
        //lightDir = L.xyz;
        vec3 lightDir = L.xyz / L.w - gl_Vertex.xyz / gl_Vertex.w;
    #else
        vec3 normal = gl_NormalMatrix * gl_Normal;
        vec3 lightDir = vec3(0,0,1);
    #endif
    
    float d = max(0.0, dot(normalize(lightDir), normal));
    color = vec4(d * gl_Color.rgb, gl_Color.a);
    }
