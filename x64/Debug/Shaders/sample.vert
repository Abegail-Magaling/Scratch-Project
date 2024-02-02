#version 330 core

layout(location = 0) in vec3 aPos;

//declare a variable to hold data

uniform mat4 transform;

void main(){
	//gl position is predefined
	//denotes the final position of the vextex / point

	gl_Position = transform * vec4(aPos, 1.0); //turns the vec3 into a vec4
}