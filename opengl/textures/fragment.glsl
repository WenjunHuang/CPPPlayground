#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float value;
void main(){
//    FragColor = texture(texture1,TexCoord) * vec4(ourColor,1.0);
    FragColor = mix(texture(texture1,TexCoord),texture(texture2,vec2(1.0-TexCoord.x,TexCoord.y)),value);
    //FragColor = ourColor;
}