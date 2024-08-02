#include "Skybox.hlsli"

struct TransformationMatrix
{
    float32_t4x4 WVP;
    float32_t4x4 World;
};


ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);



struct VertexShaderInput
{
    float32_t4 position : POSITION0;
    float32_t2 texcoord : TEXCOORD0;
    float32_t3 normal : NORMAL0;
};

VetexshaderOutput main(VertexShaderInput input)
{
    VetexshaderOutput output;
    output.position = mul(input.position, gTransformationMatrix.WVP).xyww;
    output.texcoord = input.position.xyz;

    return output;
}
//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}