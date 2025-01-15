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

struct PointLight
{
    float32_t4 color;
    float32_t3 position;
    float intensity;
    float radius;
    float decay;
};

struct Camera
{
    float32_t3 worldPosition;
};


ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);
ConstantBuffer<PointLight> gPointLight : register(b3);

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
         
        
        float32_t3 halfVector = normalize(-gDirectionalLight.direction + gPointLight.position + toEye);
        float NdotH = dot(normalize(input.normal), halfVector);
       
        float RdotE = dot(reflectLight, toEye);//u*r
        float specularPow = pow(saturate(NdotH), gMaterial.shininess);// saturate (u * r) s乗
            
             
        //float32_t3 pointLightDirection = normalize(input.worldPostion - gPointLight.position);
    
        //pow = 2乗
           
        //平行光源
        float32_t3 diffuseDirectionalLight = 
        gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        
        float32_t3 specularDirectionalLight = //Is
        gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.0f,1.0f,1.0f);
            
        
        
        float32_t distance = length(gPointLight.position - input.worldPostion);
        float32_t factor = pow(saturate(-distance / gPointLight.radius + 1.0f),gPointLight.decay);

        //ポイントライト
        float32_t3 diffusePointLight = 
        gMaterial.color.rgb * textureColor.rgb * gPointLight.color.rgb * cos * gPointLight.intensity * factor;
        
        float32_t3 specularPointLight =
        gPointLight.color.rgb * gPointLight.intensity * factor * specularPow * float32_t3(1.0f, 1.0f, 1.0f);
        
        
        
        output.color.rgb = 
        diffuseDirectionalLight + specularDirectionalLight +
        diffusePointLight + specularPointLight;
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