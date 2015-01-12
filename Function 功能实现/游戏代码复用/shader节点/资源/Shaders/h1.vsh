//uniform mat4 gl_ModelViewMatrix;
//uniform mat4 gl_ProjectionMatrix;
//attribute vec4 gl_Vertex;
//attribute vect gl_Position;

void main()
{
//	gl_Position = gl_PorjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
	gl_Position = ftransform();
}

