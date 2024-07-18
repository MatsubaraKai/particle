struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

Texture2D screenTexture : register(t0);
SamplerState samplerState : register(s0);

cbuffer ConstantBuffer : register(b0)
{
    float2 resolution;
    float chromaticAberrationAmount; // クロマティックアベレーションの強度
};

float4 main(VertexShaderOutput input) : SV_TARGET
{
    float2 texCoord = input.texcoord * resolution.xy;

    // 赤・青・緑のずれを加える
    float2 redOffset = float2(chromaticAberrationAmount, 0);
    float2 greenOffset = float2(0, chromaticAberrationAmount);
    float2 blueOffset = float2(-chromaticAberrationAmount, 0);

    float4 colorRed = screenTexture.Sample(samplerState, texCoord + redOffset);
    float4 colorGreen = screenTexture.Sample(samplerState, texCoord + greenOffset);
    float4 colorBlue = screenTexture.Sample(samplerState, texCoord + blueOffset);

    // ずれた色を組み合わせる
    float4 finalColor = float4(colorRed.r, colorGreen.g, colorBlue.b, 1.0);

    return finalColor;
}
