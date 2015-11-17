// Define used GLSL version
#version 120

varying float depth;
uniform float nearClipPlane;
uniform float farClipPlane;

void main(void) {
    gl_Position = ftransform();
    vec4 transformed = gl_ModelViewMatrix * gl_Vertex;
    float z_camera_view = -(transformed.z / transformed.w) - nearClipPlane;
    depth = z_camera_view / ( farClipPlane - nearClipPlane );
}
