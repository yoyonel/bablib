#define RETURN_INDEX

#ifdef RETURN_INDEX
    void main(out uvec2 output) {
        output = uvec2(uint(gl_PrimitiveID), 1);    // # of primitives since last glBegin
        }
#else
    void main() {   //out vec3 output) {
        //const uint v = 4294967295;
        //const uint v = 2147483647;
        //const uint v = 1;
        //gl_FragColor = vec4(v, v, 0, 0);
        //gl_FragColor = vec4(1, 1, 0, 0);
        //gl_FragColor.r = float(3);// / float(4294967295);
        //output = uvec2(2147483647, 1);
        //output = uvec2(uint(index), 1);
        
        //output = uvec2(uint(indexUnif), 1);
        //output = uvec2(uint(gl_PrimitiveID), 1);    // # of primitives since last glBegin
        //gl_FragColor = vec4(255, 255, 0, 0);
        //gl_FragColor = vec4(1, 1, 0, 0);
        //gl_FragColor = vec4(index, 1, 0, 0);
        //output = vec3(1,0,0);
        gl_FragColor = vec4(float(gl_PrimitiveID) / 100000.0, 0, 0, 1);
        }

#endif
