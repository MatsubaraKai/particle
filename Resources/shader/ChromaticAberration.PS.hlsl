// クロマティックアベレーションの強さを調整するためのパラメータ
cbuffer ChromaticAberrationBuffer : register(b0)
{
    float2 aberrationAmount; // 各カラーのオフセット量
    float edgeThreshold; // エフェクトが始まる距離の閾値
};

// 入力構造体
struct PS_INPUT
{
    float4 Pos : SV_POSITION; // ピクセルのスクリーン空間位置
    float2 Tex : TEXCOORD0; // テクスチャ座標
};

// 出力構造体
struct PS_OUTPUT
{
    float4 Color : SV_Target; // ピクセルの最終カラー
};

// テクスチャサンプラー
Texture2D inputTexture : register(t0);
SamplerState samplerState : register(s0);

// ピクセルシェーダ関数
PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;

    // 画面の中心を基準に距離を計算
    float2 screenCenter = float2(0.5, 0.5);
    float distanceFromCenter = distance(input.Tex, screenCenter);

    // 距離に基づいてエフェクトの強度を計算
    float effectStrength = saturate((distanceFromCenter - edgeThreshold) / (10.0 - edgeThreshold));

    // テクスチャ座標の範囲をクリッピング
    float2 texCoordR = clamp(input.Tex + aberrationAmount * effectStrength, 0.0, 1.0);
    float2 texCoordG = clamp(input.Tex, 0.0, 1.0);
    float2 texCoordB = clamp(input.Tex - aberrationAmount * effectStrength, 0.0, 1.0);

    // テクスチャから各チャンネルの色をサンプル
    float4 colorR = inputTexture.Sample(samplerState, texCoordR);
    float4 colorG = inputTexture.Sample(samplerState, texCoordG);
    float4 colorB = inputTexture.Sample(samplerState, texCoordB);

    // 各チャンネルの色を組み合わせて最終色を作成
    output.Color = float4(colorR.r, colorG.g, colorB.b, 1.0f);

    return output;
}
