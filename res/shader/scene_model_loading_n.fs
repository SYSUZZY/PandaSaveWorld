#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentLightDir;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_specular1;
uniform sampler2D texture_height1;
uniform float shininess;

uniform Light light;

void main()
{   
	// obtain normal from normal map in range [0,1]
    vec3 normal = texture(texture_height1, fs_in.TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space

	//ambient
	vec3 ambient = light.ambient * texture(texture_diffuse1, fs_in.TexCoords).rgb;

	//diffuse
    //vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    vec3 lightDir = normalize(-fs_in.TangentLightDir);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, fs_in.TexCoords).rgb;  

    //specular
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
    vec3 specular = light.specular * spec * texture(texture_specular1, fs_in.TexCoords).rgb;  
        
    vec3 result = ambient + diffuse + specular;
    //ambient + diffuse + specular;
    //FragColor = vec4(result, 1.0);
    FragColor = texture(texture_height1, fs_in.TexCoords);
}