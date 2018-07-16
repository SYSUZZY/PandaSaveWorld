#version 330 core
out vec4 FragColor;

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos; 
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D shadowMap;
uniform float shininess;

uniform vec3 viewPos;
uniform Light light;
uniform vec3 lightPos;
uniform vec3 lightDirection;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // 执行透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;
    // 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // 取得当前片元在光源视角下的深度
    float currentDepth = projCoords.z;

    // 优化，使用一定的偏移
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // 检查当前片元是否在阴影中
    //float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(texture_diffuse2, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap,projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{    
	//ambient
	vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;

	//diffuse
	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    //vec3 lightDir = normalize(-lightDirection);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse2, TexCoords).rgb;  

    //specular
	vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    vec3 halfwayDir = normalize(lightDir + viewDir); 
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), shininess);
    vec3 specular = light.specular * spec * texture(texture_diffuse1, TexCoords).rgb;  
    
    // 计算阴影
    float shadow = ShadowCalculation(FragPosLightSpace);       
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * texture(texture_diffuse1, TexCoords).rgb;  
    FragColor = vec4(lighting , 1.0);
    //FragColor = texture(texture_diffuse1, TexCoords);
}