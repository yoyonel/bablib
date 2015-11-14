varying vec3 worldViewDir;  // ~ (p1 - p0)
varying vec3 p0;
varying vec3 p1;
varying vec3 dx0;
varying vec3 dx1;
varying vec3 dy0;
varying vec3 dy1;

//const float unit = 1.0;           // taille d'une unité de la grille
const float unit = 10.0;           // taille d'une unité de la grille
const float thickness = 1.1;

//#define FOCUS_RADIUS 500.0
//#define HORIZ_HAZE  0.01
//#define COMPUTE_DEPTH
#define ZMIN 0.0
#define ZMAX 0.99999997
//#define ZMAX 1.0 - 1.0 / (1 << 24)

const vec4 colorGround = vec4(0.3.xxx,1);
//const vec4 colorGrid   = 0.6*vec4(1,1,1,1);
const vec4 colorGrid   = vec4(1,0.8,0,1);

const vec4 colorSky = vec4(0.3.xxx,1);
//const vec4 colorSky = vec4(0.25.xxx,1);
//const vec4 colorSky = vec4(0.25, 0.35, 0.45, 1);
//const vec4 colorSky = vec4(0,0,0,1);
const vec4 colorHaze = colorSky;
//const vec4 colorHaze = vec4(0.25, 0.35, 0.45, 1);

vec2 gate(vec2 x, vec2 sigma) { return exp(-x*x / (2*sigma*sigma)); }
//vec2 gate(vec2 x, vec2 sigma) { return 1 - smoothstep(0.0, 2*sigma, x); }     // moins couteux que la gaussienne
//vec2 gate(vec2 x, vec2 sigma) { return max(1 - x / (2*sigma), 0.0); }         // encore moins couteux

void main() {
    //const vec3 worldViewDir = p1 - p0;
    if (worldViewDir.z < 0) {   // mapping de la cube map sur le plan z=0
    //if (worldViewDir.z < 0) {   // mapping de la cube map sur le plan z=0
        vec3 q = p0 - (p0.z / worldViewDir.z) * worldViewDir;   // point sur le plan z=0
        // q = mix(p0, p1, alpha);        
        
        #ifdef COMPUTE_DEPTH
            // calcul du z effectif :
            vec4 pixEye = gl_ModelViewProjectionMatrix * vec4(q, 1);
            float z = pixEye.z / pixEye.w;      // dans [-1,1]
            gl_FragDepth = clamp((z + 1) / 2, ZMIN, ZMAX);
        #endif
        
        // dessin de la  grille :
        //-----------------------
            // le calcul suivant marche ssi le near plane et le far plane sont parallèles
            float alpha = -p0.z / (p1.z - p0.z);
            vec3 dx = mix(dx0, dx1, alpha);     // vecteur pixel vertical
            vec3 dy = mix(dy0, dy1, alpha);     // vecteur pixel horizontal
            dx -= (dx.z / worldViewDir.z) * worldViewDir;   // projection de dx sur le sol
            dy -= (dy.z / worldViewDir.z) * worldViewDir;   // projection de dy sur le sol
            
            //vec2 s = max(abs(dx), abs(dy));
            //vec2 s = abs(dx) + abs(dy);
            vec2 s = sqrt(dx * dx + dy * dy);
            //@ revoir ce calcul : convolution ellipse * grille + calcul exact de l'ellipse + offset de g car ellipse decentree ..
            
            vec2 g = fract(q.xy / unit);      // point ramené à [0,1]²
            //g.x = fract(degrees(atan(q.y,q.x)) / (20 * unit));
            //g.y = fract(length(q) / unit);
            vec2 d = thickness * s / unit;
            vec2 p = gate(g, d) + gate(1-g, d);
            //vec2 p = 1 - (1 - gate(g,d)) * (1 - gate(1-g,d));
            p /= 1 + 60*d;
            //p *= 0.01 + 1.0 / (1.0 + 80*d);
            //p = mix(0.01, p, 1.0 / (1 + 40*d));
            //p /= max(1.0, (0 + 10*s));
            
            //float grid = p.x * p.y;
            //float grid = 1.0 - (1-p.x) * (1-p.y);
            float grid = (1-p.x) * (1-p.y);
            //float grid = 1 - (p.x + p.y);
            //float grid = min(1 - p.x, 1 - p.y);
            
        gl_FragColor = mix(colorGrid, colorGround, grid);
    
        // lighting diffus :
            //gl_FragColor.rgb *= mix(0.4, 1.0, - normalize(worldViewDir).z);
            //gl_FragColor = mix(colorSky, gl_FragColor, - normalize(worldViewDir).z);
        
        #ifdef FOCUS_RADIUS
            float a = max(0.0f, length(q.xy) / FOCUS_RADIUS - 1.0);
            //float a = 0.01*(length(q - mix(p0,p1, dx0.x / (dx1.x-dx0.x))));     //@ moche : pour retrouver la position de l'oeil
            //gl_FragColor = mix(colorSky, mix(colorGrid, colorGround, grid), exp(-a));        
            gl_FragColor = mix(colorHaze, mix(colorGrid, colorGround, grid), exp(-a));        
        #endif

        #ifdef HORIZ_HAZE
            float horiz = -worldViewDir.z / length(worldViewDir);
            gl_FragColor = mix(colorHaze, gl_FragColor, clamp(horiz / HORIZ_HAZE, 0.0f, 1.0f));
        #endif
        }
    else {
        #ifdef HORIZ_HAZE
            float horiz = worldViewDir.z / length(worldViewDir);
            gl_FragColor = mix(colorHaze, colorSky, clamp(horiz / HORIZ_HAZE, 0.0f, 1.0f));
        #else
            gl_FragColor = colorSky;
        #endif

        #ifdef COMPUTE_DEPTH
            gl_FragDepth = ZMAX;
        #endif
        
        /*
        // affichage hemisphère supérieure :
        float grid = 1.0f;
        //float dist = (worldViewDir.z / length(worldViewDir));
        //float dist = 0.02 * length(q - p0) / (abs(worldViewDir.z) / length(worldViewDir));
    
        float PI = 3.1416;
        vec2 q = vec2(
            atan(worldViewDir.y, worldViewDir.x) / (2 * PI),
            atan(worldViewDir.z, length(worldViewDir.xy)) / (2 * PI)
            );
        
        // dessin des grilles :
        const float m = 0.01;
        const float eps = 0.1;
        //const float eps = 0.03 * dist;
        //if (any(fract(m*p.yz) < eps)) diffuseColor *= 0.5;
        vec2 p = smoothstep(0.0f, eps, fract(q.xy / m)) * (1.0f - smoothstep(1.0f-eps, 1.0f, fract(q.xy / m)));
        //p = 0.8 + 0.2*p;
        p = 0.6 + 0.4*p;
        p = 2 - p;
        //p = mix(vec2(1.0f, 1.0f), p, 1 / (1 + dist));
        
        grid *= p.x;
        grid *= p.y;
        //grid *= max(p.x, p.y);
        
        gl_FragColor.rgb *= grid;
        */
        }    
    }

