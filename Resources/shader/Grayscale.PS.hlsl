#include "Fullscreen.hlsli"

struct Material
{
    float32_t3 projectionInverse;
};


ConstantBuffer<Material> gMaterial : register(b0);

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);
    float32_t value = dot(output.color.rgb, float32_t3(gMaterial.projectionInverse.x, gMaterial.projectionInverse.y, gMaterial.projectionInverse.z));
    output.color.rgb = float32_t3(value, value, value);
    return output;

};