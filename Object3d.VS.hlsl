#include"Object3d.hlsli"

struct TransformationMatrix
{
    float32_t4x4 WVP;
};
ConstantBuffer<TransformationMatrix> gTranfsformationMatrix : register(b0);

//struct VertexShaderOutput
//{
//    float32_t4 position : SV_POSITION;
//};

struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
};
struct Material
{
    float32_t4 color;
    int32_t enableLighting;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.position = mul(input.position, gTranfsformationMatrix.WVP);
    output.texcoord = input.texcoord;
    return output;
}