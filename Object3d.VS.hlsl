float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}

struct VertexShaderOutput
{
    float32_t4 posision : SV_POSITION;
};

struct VertexShaderInput
{
    float32_t4 posision : POSITION0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    output.posision = input.posision;
    return output;
}