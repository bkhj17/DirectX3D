#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 viewDir : VIEWDIR;
};

Texture2D ambientMap : register(t2);
cbuffer IntensityBuffer : register(b1)
{
    float shininessIntensity;
};

PixelInput VS(VertexUVNormal input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) world); // 
    //output.viewDir = normalize(output.pos.xyz - invView._41_42_43);   //
    output.viewDir = normalize(invView._31_32_33); //forward    
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 albedo = diffuseMap.Sample(samp, input.uv);
    float3 normal = normalize(input.normal);
    float3 light = normalize(lightDirection);
    
    float diffuseIntensity = saturate(dot(normal, -light));

    float4 specular = 0;
    if (diffuseIntensity > 0)
    {
        //Phong Shading : �̷� �״���� ��Ȯ�� ����
        //float3 reflection = normalize(reflect(light, normal));  //�ݻ簢
        //specular = saturate(dot(-reflection, input.viewDir));
        
        //Blinn Phong Shading(������) : ������ ��ġ�� ȿ�������� ���� ����
        float3 halfWay = normalize(input.viewDir + light);
        specular = saturate(dot(normal, -halfWay));
        
        float4 specularIntensity = specularMap.Sample(samp, input.uv);
                
        specular = pow(specular, shininess * shininessIntensity) * specularIntensity;
    }
    
    float4 diffuse = albedo * diffuseIntensity;
    //������
    float4 ambient = albedo * ambientMap.Sample(samp, input.uv);
    
    return max(ambient, diffuse + specular);
}