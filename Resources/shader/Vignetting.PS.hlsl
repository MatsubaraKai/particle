#include "Fullscreen.hlsli"
struct Material
{
    float32_t2 projectionInverse;
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
    
    // 周囲を0に、中心になるほど明るくなるように計算で調整
    float32_t2 correct = input.texcoord * (1.0f - input.texcoord.yx);
    // correctだけで計算すると中心の最大値が0.0625で暗すぎるのでScaleで調整。この例では16倍して1にしている
    float vignette = correct.x * correct.y * gMaterial.projectionInverse.x;
    // とりあえず0.8乗でそれっぽくしてみた
    vignette = saturate(pow(vignette, gMaterial.projectionInverse.y));
    // 係数として乗算
    output.color.rgb *= vignette;
    return output;

};