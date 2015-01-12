uniform mat4 u_matViewProjection;
attribute vec4 a_position;
attribute vec2 a_texCoord0;
varying vec2 v_texCoord; // Varying in vertex shader
void main(void)
{
gl_Position = u_matViewProjection * a_position;
v_texCoord = a_texCoord0;
}
