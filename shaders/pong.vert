#version 450

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec3 a_Color;

layout(location = 0) out vec3 v_Color;

layout (push_constant) uniform Push
{
	mat4 Projection;
	mat4 Transform;
} u_Push;

void main()
{
	v_Color = a_Color;

	gl_Position = u_Push.Projection * u_Push.Transform * vec4(a_Position, 0.0, 1.0);
}