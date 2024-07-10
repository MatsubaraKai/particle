#include "Fullscreen.hlsli"

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

float gScanlineIntensity = 0.1f; // 走査線の強度

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;

    // ベースの色をテクスチャからサンプルする
    float4 baseColor = gTexture.Sample(gSampler, input.texcoord);

    // 時間に応じて走査線の位置を変化させる
    float time = 0.0f; // 時間経過を表す変数
    float scanline = frac((input.texcoord.y + time) * 1000.0f); // 1000は走査線の間隔を調整する係数

    // 走査線のパターンを加える
    baseColor.rgb -= baseColor.rgb * scanline * gScanlineIntensity;

    output.color = baseColor;

    return output;
}
