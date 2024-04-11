#version 460 core
in vec2 uv;
in vec3 color;
out vec4 FragColor;

uniform sampler2D sampler;
void main(){
    FragColor = texture(sampler, uv);
}