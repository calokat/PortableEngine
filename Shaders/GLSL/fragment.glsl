#version 430

in vec3 color;

// uniform sampler2D ourTexture;

out vec4 out_color;
void main()
{
    out_color = vec4(color, 1);
    // out_color = texture(ourTexture, TexCoord);
}
