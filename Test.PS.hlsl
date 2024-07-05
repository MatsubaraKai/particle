#include "Fullscreen.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

static const int KernelSize = 3;

static const float2 kIndex3x3[3][3] =
{
    { { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
    { { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f } },
    { { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } },
};

static const float kPrewittHorizontalKernel[3][3] =
{
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
};

static const float kPrewittVerticalKernel[3][3] =
{
    { -1.0f / 6.0f, -1.0f / 6.0f, -1.0f / 6.0f },
    { 0.0f, 0.0f, 0.0f },
    { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f },
};

float Luminance(float3 v)
{
    return dot(v, float3(0.2125f, 0.7154f, 0.0721f));
};

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    // LuminanceBasedOutline の処理
    uint width, height;
    gTexture.GetDimensions(width, height);
    float2 uvStepSize = float2(rcp(width), rcp(height));
    
    float2 difference = float2(0.0f, 0.0f);
    for (int x = 0; x < KernelSize; ++x)
    {
        for (int y = 0; y < KernelSize; ++y)
        {
            float2 texcoord = input.texcoord + kIndex3x3[x][y] * uvStepSize;
            float3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
            float luminance = Luminance(fetchColor);
            difference.x += luminance * kPrewittHorizontalKernel[x][y];
            difference.y += luminance * kPrewittVerticalKernel[x][y];
        }
    }
    
    float weight = length(difference);
    weight = saturate(weight * 6.0f);

    float3 outlineColor = (1.0f - weight) * gTexture.Sample(gSampler, input.texcoord).rgb;

    // RadialBlur の処理
    const float2 kCenter = float2(0.5f, 0.5f);
    const int kNumSamples = 10;
    const float kBlurWidth = 0.01f;
    
    float2 direction = input.texcoord - kCenter;
    float3 blurredColor = float3(0.0f, 0.0f, 0.0f);
    for (int sampleIndex = 0; sampleIndex < kNumSamples; ++sampleIndex)
    {
        float2 texcoord = input.texcoord + direction * kBlurWidth * float(sampleIndex);
        blurredColor += gTexture.Sample(gSampler, texcoord).rgb;
    }
    blurredColor *= rcp(kNumSamples);

    // 両方の効果を合成
    float3 finalColor = outlineColor * 0.5f + blurredColor * 0.5f;

    PixelShaderOutput output;
    output.color.rgb = finalColor;
    output.color.a = 1.0f;

    return output;
};
