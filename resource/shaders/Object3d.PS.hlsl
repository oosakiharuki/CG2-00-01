#include"Object3d.hlsli"


//float4 main() : SV_TARGET
//{
//    return float4(1.0f, 1.0f, 1.0f, 1.0f);
//}

struct Material
{
    float32_t4 color;
    int32_t enableLighting;
    float32_t4x4 uvTransform;
    float32_t shininess;
};

struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};

struct Camera
{
    float32_t3 worldPosition;
};


ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);

Texture2D<float32_t4> gTexture : register(t0);

SamplerState gSampler : register(s0);



struct PixelShaderOutput
{
    float32_t4 color : SV_Target0; 
};

PixelShaderOutput main(VertexShaderOutput input)
{
    float4 transformdUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformdUV.xy);
    
    PixelShaderOutput output;
    
    if (gMaterial.enableLighting != 0)
    {
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f , 2.0f);
        //output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
    
        //output.color.rgb = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        //output.color.a = gMaterial.color.a * textureColor.a;
            
        float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPostion);
        float32_t reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
    
        
        float RdotE = dot(reflectLight, toEye);//u*r
        float specularPow = pow(saturate(RdotE), gMaterial.shininess);// saturate (u * r) s乗
            
       
        //pow = 2乗
           

        float32_t3 diffuse = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        
        float32_t3 specular = //Is
        gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.0f,1.0f,1.0f);
        
        output.color.rgb = diffuse + specular;
        output.color.a = gMaterial.color.a * textureColor.a;
        
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }

    if (textureColor.a <= 0.5)
    {
        discard;
    }
    
    if (textureColor.a == 0.0)
    {
        discard;
    }
    
    if (output.color.a == 0.0)
    {
        discard;
    }
    
    
    return output;
}