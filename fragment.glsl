#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_height1;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;

void main()
{
    // 环境光
    vec3 diffuseColor = texture2D(material.texture_diffuse1, TexCoord).rgb;
    vec3 specularColor = texture2D(material.texture_specular1, TexCoord).rgb;

    vec3 ambient  = light.ambient * diffuseColor;

    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse  = light.diffuse * (diff * diffuseColor);

    // 镜面光
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * specularColor);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
