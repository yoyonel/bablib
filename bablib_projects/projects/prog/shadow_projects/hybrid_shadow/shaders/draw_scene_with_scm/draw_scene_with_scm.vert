// Define used GLSL version
#version 120

// le qualifieur "flat" indique au vertex processor de ne pas interpoler le paramÃÂÃÂ¨tre (entre les 3 points dÃÂÃÂ©finissant le triangle)
// Selon la convention spÃÂÃÂ©cifiÃÂÃÂ©e par glProvokingVertex( enum provokeMode) avec provokeMode = {FIRST_VERTEX_CONVENTION, LAST_VERTEX_CONVENTION},
// on prend (respectivement) le 1er point ou dernier point comme rÃÂÃÂ©fÃÂÃÂ©rence d'attribut (la couleur dans cet exemple)
flat varying vec4 object_color;

void main(void) {
	gl_Position = ftransform();
	object_color = gl_Color;
}
