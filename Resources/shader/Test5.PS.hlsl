#include "Fullscreen.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

static const int32_t KenelSize = 3;

static const float32_t2 kIndex3x3[3][3] =
{
    { { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
    { { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f } },
    { { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } },
};

static const float32_t kPrewittHorizontalKernel[3][3] =
{
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
};

static const float32_t kPrewittVerticalKernel[3][3] =
{
    { -1.0f / 6.0f, -1.0f / 6.0f, -1.0f / 6.0f },
    { 0.0f, 0.0f, 0.0f },
    { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f },
};

float32_t Luminance(float32_t3 v)
{
    return dot(v, float32_t3(0.2125f, 0.7154f, 0.0721f));
};

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    uint width, height;
    gTexture.GetDimensions(width, height);
    float2 uvStepSize = float2(1.0f / width, 1.0f / height);
    
    float2 difference = float2(0.0f, 0.0f);
    
    for (int x = 0; x < KenelSize; ++x)
    {
        for (int y = 0; y < KenelSize; ++y)
        {
            float2 texcoord = input.texcoord.xy + kIndex3x3[x][y] * uvStepSize;
            float3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
            float luminance = Luminance(fetchColor);
            difference.x += luminance * kPrewittHorizontalKernel[x][y];
            difference.y += luminance * kPrewittVerticalKernel[x][y];
        }
    }
    
    float weight = length(difference);
    weight = saturate(weight * 100.0f);
    
    // アウトラインは黒色
    float4 outlineColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    // エッジの検出結果を取得
    float3 edgeColor = lerp(outlineColor.rgb, float3(0.0f, 0.7f, 1.0f), weight);

    // ブラー効果を適用する
    const float2 kCenter = float2(0.5f, 0.5f); // 中心点。ここを基準に放射状にブラーがかかる
    const int kNumSamples = 10; // サンプリング数。多いほど滑らかだが重い
    const float kBlurWidth = 0.01f; // ぼかしの幅。大きいほど大きい

    float2 direction = input.texcoord - kCenter;
    float3 blurredColor = float3(0.0f, 0.7f, 1.0f);
    float2 texcoord;

    for (int sampleIndex = 0; sampleIndex < kNumSamples; ++sampleIndex)
    {
        texcoord = input.texcoord + direction * kBlurWidth * float(sampleIndex);
        blurredColor += gTexture.Sample(gSampler, texcoord).rgb;
    }

    blurredColor /= kNumSamples;

    PixelShaderOutput output;
    
    // エッジ部分にブラー効果を適用
    output.color.rgb = lerp(edgeColor, blurredColor, weight);
    output.color.a = 0.0f;

    return output;
}
