#version 330 core
out vec4 FragColor;

in vec3 fragPos;
in vec3 normCoord;
in vec2 texCoord;
in mat3 TBN;

uniform sampler2D diffuseMap;
uniform sampler2D overlayMap;
uniform sampler2D normalMap;
uniform vec3 cameraPos;
uniform float tiling;
uniform bool useAlphaClip;

struct PointLight {
    vec3 position;  
	vec3 color;     
	float strength;

    float ambientStr; 
	vec3 ambientColor;

    float specularStr; 
	float specularPhong;

    float constant; 
	float linear; 
	float quadratic; 
	float range;
};

struct DirLight {
    vec3 direction; 
	vec3 color;    
	float strength;

    float ambientStr; 
	vec3 ambientColor;

    float specularStr; 
	float specularPhong;
};

uniform PointLight pointLight;
uniform DirLight   dirLight;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 texColor)
{
	vec3 lightDir = normalize(light.position - fragPos);
    float distance = length(light.position - fragPos);

	float att = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    float rangeAtt = clamp(1.0 - (distance / light.range), 0.0, 1.0);
    rangeAtt *= rangeAtt; // Quadratic for a nicer curve (optional)
    att *= rangeAtt;

	vec3 ambient = light.ambientStr * light.ambientColor * texColor;

	float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * light.color * light.strength * texColor;

	vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfDir), 0.0), light.specularPhong);
	vec3 specular = light.specularStr * spec * light.color;

	return(ambient + diffuse + specular) * att;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 texColor)
{
    vec3 lightDir = normalize(-light.direction);

    vec3 ambient = light.ambientStr * light.ambientColor * texColor;

    float diff    = max(dot(normal, lightDir), 0.0);
    vec3  diffuse = diff * light.color * light.strength * texColor;

    vec3  halfDir  = normalize(lightDir + viewDir);
    float spec     = pow(max(dot(normal, halfDir), 0.0), light.specularPhong);
    vec3  specular = light.specularStr * spec * light.color;

    return ambient + diffuse + specular;
}


void main()
{
    vec2 tiledUV = vec2(texCoord.x, texCoord.y) * tiling;
	vec4 baseColor = texture(diffuseMap, tiledUV);
    if(baseColor.a < 0.1) { discard; }

    if (useAlphaClip && (baseColor.r + baseColor.g + baseColor.b) < 0.15) { discard; }

    vec4 overlayColor = texture(overlayMap, texCoord);

    vec3 blendedRGB = mix(baseColor.rgb, overlayColor.rgb, overlayColor.a);
    float blendedA  = mix(0.5, 1.0, overlayColor.a);

    vec3 normal = texture(normalMap, tiledUV).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(TBN * normal);
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 result = vec3(0.0);
    result += CalcPointLight(pointLight, normal, fragPos, viewDir, vec3(0.f, blendedRGB.y, 0.f));
    result += CalcDirLight(dirLight, normal, viewDir, vec3(0.f, blendedRGB.y, 0.f));

    FragColor = vec4(result, blendedA);


    /*
    //Debug

    vec4 baseColor    = texture(diffuseMap, texCoord);
    vec4 overlayColor = texture(overlayMap, texCoord);

    vec3 blendedRGB = mix(baseColor.rgb, overlayColor.rgb, overlayColor.a);
    float blendedA  = mix(0.5, 1.0, overlayColor.a);

    FragColor = vec4(blendedRGB, blendedA);
    */
}