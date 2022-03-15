#version 300 es
precision mediump float;

struct PointLight {
    vec4 AmbientColor;
    vec4 DiffuseColor;
    vec3 Position;
    float Intensity;
};

struct DirectionalLight {
    vec4 AmbientColor;
    vec4 DiffuseColor;
    vec4 Direction;
};

in vec4 color;
in vec3 normal;
in vec3 worldPos;

uniform DirectionalLight dirLight;
uniform PointLight pointLights[8];
uniform vec3 cameraPos;
uniform float specularIntensity;

out vec4 out_color;

float PhongPoint(PointLight light)
{
    vec3 R = normalize(reflect(worldPos - light.Position.xyz, normal));
    vec3 V = normalize(cameraPos - worldPos);
    float RdotV = clamp(dot(R, V), 0.0, 1.0);
    float spec = pow(RdotV, 256.0);
    spec = spec * specularIntensity;
    return spec;
}

vec3 CalculatePointLight(PointLight light)
{
    vec3 normalizedNormal = normalize(normal);
    vec3 normalizedNegatedLightDir = normalize(light.Position.xyz - worldPos);
    float lightAmount = clamp(dot(normalizedNegatedLightDir, normalizedNormal), 0.0, 1.0);
    lightAmount = lightAmount * light.Intensity / distance(light.Position, worldPos);
    vec4 finalColor = lightAmount * vec4(light.DiffuseColor.xyz, 1) * color + vec4(light.AmbientColor.xyz, 1) + PhongPoint(light);
    return finalColor.xyz;
}

vec3 CalculateDirLight(DirectionalLight light)
{
    vec3 normalizedNormal = normalize(normal);
	vec3 normalizedNegatedLightDir = normalize(-light.Direction.xyz);
    float lightAmount = clamp(dot(normalizedNegatedLightDir, normalizedNormal), 0.0, 1.0);
	vec4 finalColor = lightAmount * light.DiffuseColor * color + light.AmbientColor * color;
	return finalColor.xyz;
}


void main()
{
    vec3 finalColor = CalculateDirLight(dirLight) + CalculatePointLight(pointLights[0]);
    finalColor = finalColor * color.xyz;
    out_color = vec4(finalColor, 1);
}