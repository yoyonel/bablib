varying vec3 origRay;
varying vec3 dirRay;

uniform vec3 center;
uniform float radius;
uniform vec3 point;

float gaussian(float x) { return exp(-x*x); }
float splat(vec3 p, vec3 q) {
    //@ use sphere radius
    float x = 10 * length(p - q);
    return clamp(gaussian(x), 0.0, 1.0);
    }

void main() {
    //float r = radius;
    float eta = 1.0 / 1.5;

    vec3 p = (origRay - center) / radius;
    vec3 d = normalize(dirRay);
    
    float b = dot(p, d),
          c = dot(p, p) - 1.0,
          delta = b*b - c;
    
    if (delta < 0.0) discard;
    
    float t = -b - sqrt(delta);
    vec3 q = p + t*d;     // point on the sphere
    vec3 n = normalize(q);      // normal at this point //@@ == q
    
    //vec3 q = (p - center) / radius;
    float s = splat(q, point);
    
    float diffuse = max(0.0, dot(n, -d));
    //gl_FragColor = vec4(diffuse * vec3(1,0,0), 0.5);
    //gl_FragColor = vec4(diffuse * mix(vec3(1,0,0), vec3(1,1,1), clamp(10*abs(length(p - point)), 0.0, 1.0)), 0.5);
    //gl_FragColor = vec4(mix(vec3(0.4,0.7,1), vec3(1,1,0), s), max(1-diffuse, s));
    //gl_FragColor = vec4(diffuse * mix(vec3(0.4,0.7,1), vec3(1,1,0), s), max(1-diffuse, s));
    //gl_FragColor = vec4(mix(diffuse * vec3(0.4,0.7,1), vec3(1,1,0), s), max(1-diffuse, s));
    gl_FragColor = vec4(mix(mix(0.25, 1.0, diffuse) * vec3(0.4,0.7,1), vec3(1,1,0), s), max(1-diffuse, s));
    //gl_FragColor = vec4(vec3(1,1,1), 1-diffuse);
    
    // computation of exact depth :
    vec4 pe = gl_ModelViewProjectionMatrix * vec4(center + radius * q, 1);
    gl_FragDepth = (1 + pe.z / pe.w) / 2;
    }

