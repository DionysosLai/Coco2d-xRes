// Fragment shader
precision mediump float;
varying vec2 v_texCoord; // Varying in fragment shader
uniform sampler2D s_baseMap;
uniform sampler2D s_lightMap;
void main()
{
vec4 baseColor;
vec4 lightColor;
baseColor = texture2D(s_baseMap, v_texCoord);
lightColor = texture2D(s_lightMap, v_texCoord);
gl_FragColor = baseColor * (lightColor + 0.25);
}