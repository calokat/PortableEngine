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

struct SpotLight {
    vec4 AmbientColor;
    vec4 DiffuseColor;
    mat4 InverseOrientation;
    vec3 Position;
    float Intensity;
    float Angle;
    float Range;
    vec2 padding;
};

in vec4 color;
in vec3 normal;
in vec3 worldPos;
in vec2 TexCoord;

uniform DirectionalLight dirLight;
uniform PointLight pointLights[8];
uniform SpotLight spotLight;
uniform vec3 cameraPos;
uniform float specularIntensity;
uniform sampler2D ourTexture;

out vec4 out_color;

float PhongPoint(PointLight light)
{
    vec3 R = normalize(reflect(worldPos - light.Position.xyz, normal));
    vec3 V = normalize(cameraPos - worldPos);
    float RdotV = clamp(dot(R, V), 0.0, 1.0);
    float spec = pow(RdotV, 32.0);
    // spec = spec * specularIntensity;
    return spec;
}

vec3 CalculatePointLight(PointLight light)
{
    vec3 normalizedNormal = normalize(normal);
    vec3 normalizedNegatedLightDir = normalize(light.Position.xyz - worldPos);
    float lightAmount = clamp(dot(normalizedNegatedLightDir, normalizedNormal), 0.0, 1.0);
    lightAmount = lightAmount * light.Intensity / distance(light.Position, worldPos);
    vec4 finalColor = lightAmount * vec4(light.DiffuseColor.xyz, 1) * color + vec4(light.AmbientColor.xyz, 1);
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

vec3 CalculateSpotLight(SpotLight light)
{
    vec3 normalizedNormal = normalize(normal);
    vec3 localFwd = light.InverseOrientation[2].xyz;
    float angleAffinity = dot(localFwd, normalize(worldPos - light.Position)) * (light.Angle / 90.0);
    float lightAmount = (light.Range / distance(worldPos, light.Position)) * angleAffinity;
    lightAmount = lightAmount * max(0.0, (angleAffinity - .2) * light.Intensity);
    vec4 finalColor = lightAmount * light.DiffuseColor * color + light.AmbientColor * color;
    return finalColor.xyz;
}

void main()
{
    vec3 finalColor = CalculateDirLight(dirLight) + CalculatePointLight(pointLights[0]) + CalculateSpotLight(spotLight);
    finalColor = finalColor * texture(ourTexture, TexCoord).xyz * color.xyz;
    out_color = vec4(finalColor, 1);
}