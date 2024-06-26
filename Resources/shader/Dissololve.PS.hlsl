#include "Fullscreen.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float32_t> gMaskTexture : register(t2);
SamplerState gSampler : register(s0);

static const int32_t KenelSize = 5;

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};
PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color.rgb = float32_t3(0.0f, 0.0f, 0.0f);
    output.color.a = 1.0f;
    float32_t mask = gMaskTexture.Sample(gSampler, input.texcoord);
        // マスクのしきい値を調整
    float32_t threshold = 0.5f;
    // maskの値が0.5f（闘値）以下の場合はdiscardして抜く
    if (mask <= threshold)
    {
        discard;
    }
    // Edgeっぽさを検出
    float32_t edge = 1.0f - smoothstep(threshold, threshold + 0.03f, mask);
    output.color = gTexture.Sample(gSampler, input.texcoord);
    // Edgeっぽいほど指定した色を加算
    output.color.rgb += edge * float32_t3(0.2f, 0.2f, 1.0f);
    return output;
};