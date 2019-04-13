cbuffer lightBuffer : register(b0)
{
    float3 ambientLightColor;
    float ambientLightStrength;

    float4 dynamicLightColor[8];
    float4 dynamicLightPosition[8];
    float dynamicLightStrength;
    

    float dynamicLightAttenuation_a;
    float dynamicLightAttenuation_b;
    float dynamicLightAttenuation_c;
    float numberOfLights;
};

struct PS_INPUT
{
    float4 inPosition : SV_POSITION;
    float2 inTexCoord : TEXCOORD;
    float3 inNormal : NORMAL;
    float3 inWorldPos : WORLD_POSITION;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState objSamplerState : SAMPLER : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 sampleColor = objTexture.Sample(objSamplerState, input.inTexCoord);
    float3 normalSampleColor = input.inNormal;

    float3 ambientLight = ambientLightColor * ambientLightStrength;

    float3 appliedLight = ambientLight;
    
    float3 vectorToLight = normalize(float3(dynamicLightPosition[0].x, dynamicLightPosition[0].y, dynamicLightPosition[0].z) - input.inWorldPos);
    float3 diffuseLightIntensity = max(dot(vectorToLight, input.inNormal), 0);

    float distanceToLight = distance(float3(dynamicLightPosition[0].x, dynamicLightPosition[0].y, dynamicLightPosition[0].z), input.inWorldPos);
    float attenuationFactor = 1 / (dynamicLightAttenuation_a + dynamicLightAttenuation_b * distanceToLight + dynamicLightAttenuation_c * pow(distanceToLight, 2));

    diffuseLightIntensity *= attenuationFactor;

    float3 diffuseLight = diffuseLightIntensity * float3(dynamicLightColor[0].x, dynamicLightColor[0].y, dynamicLightColor[0].z) * dynamicLightStrength;

    appliedLight += diffuseLight;

    float3 finalColor = sampleColor * appliedLight;

    return float4(finalColor, 1.0f);
}