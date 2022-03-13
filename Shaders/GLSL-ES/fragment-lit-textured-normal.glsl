#version 300 es
precision mediump float;

struct PointLight {
    vec4 AmbientColor;
    vec4 DiffuseColor;
    vec4 Position;
};

struct DirectionalLight {
    vec4 AmbientColor;
    vec4 DiffuseColor;
    vec4 Direction;
};

in vec4 color;
in mat3 TBN;
in vec3 worldPos;
in vec2 TexCoord;
// in vec3 normal;
in vec3 tangent;

uniform DirectionalLight dirLight;
uniform PointLight pointLights[8];
uniform vec3 cameraPos;
uniform float specularIntensity;
uniform sampler2D ourTexture;
uniform sampler2D normalTexture;

out vec4 out_color;

float PhongPoint(PointLight light, vec3 normal)
{
    vec3 R = normalize(reflect(worldPos - light.Position.xyz, normal));
    vec3 V = normalize(cameraPos - worldPos);
    float RdotV = clamp(dot(R, V), 0.0, 1.0);
    float spec = pow(RdotV, 32.0);
    // spec = spec * specularIntensity;
    return spec;
}

vec3 CalculatePointLight(PointLight light, vec3 normal)
{
    vec3 normalizedNormal = normalize(normal);
    vec3 normalizedNegatedLightDir = normalize(light.Position.xyz - worldPos);
    float lightAmount = clamp(dot(normalizedNegatedLightDir, normalizedNormal), 0.0, 1.0);
    vec4 finalColor = lightAmount * vec4(light.DiffuseColor.xyz, 1) * color + vec4(light.AmbientColor.xyz, 1) + PhongPoint(light, normal);
    return finalColor.xyz;
}

vec3 CalculateDirLight(DirectionalLight light, vec3 normal)
{
    vec3 normalizedNormal = normalize(normal);
	vec3 normalizedNegatedLightDir = normalize(-light.Direction.xyz);
    float lightAmount = clamp(dot(normalizedNegatedLightDir, normalizedNormal), 0.0, 1.0);
	vec4 finalColor = lightAmount * light.DiffuseColor * color + light.AmbientColor * color;
	return finalColor.xyz;
}


void main()
{
    vec3 normal = texture(normalTexture, TexCoord).xyz;
    normal = normal * 2.0 - vec3(1.0, 1.0, 1.0);
    normal = TBN * normal;
    vec3 finalColor = CalculateDirLight(dirLight, normal) + CalculatePointLight(pointLights[0], normal);
    finalColor = finalColor * texture(ourTexture, TexCoord).xyz * color.xyz;
    out_color = vec4(finalColor, 1);
}