#version 460 core

in vec2 texCoordToFragment;

uniform sampler2D fragmentTexture;

out vec4 FragColor;

void main() {
   FragColor = texture(fragmentTexture, texCoordToFragment);
}