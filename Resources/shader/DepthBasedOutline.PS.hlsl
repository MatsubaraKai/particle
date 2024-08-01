#include "Fullscreen.hlsli"
struct Material
{
    float32_t4x4 projectionInverse;
    float32_t farClip;
};


ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
Texture2D<float32_t> gDepthTexture : register(t1);
SamplerState gSampler : register(s0);
SamplerState gSamplerPoint : register(s1);

static const int32_t KenelSize = 3;

static const float32_t2 kIndex3x3[3][3] =
{
    { { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f } },
    { { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f } },
    { { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f } },
	   
};

static const float32_t2 kIndex5x5[5][5] =
{
    { { -2.0f, -2.0f }, { -1.0f, -2.0f }, { 0.0f, -2.0f }, { 1.0f, -2.0f }, { 2.0f, -2.0f } },
    { { -2.0f, -1.0f }, { -1.0f, -1.0f }, { 0.0f, -1.0f }, { 1.0f, -1.0f }, { 2.0f, -1.0f } },
    { { -2.0f, 0.0f }, { -1.0f, 0.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 2.0f, 0.0f } },
    { { -2.0f, 1.0f }, { -1.0f, 1.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 2.0f, 1.0f } },
    { { -2.0f, 2.0f }, { -1.0f, 2.0f }, { 0.0f, 2.0f }, { 1.0f, 2.0f }, { 2.0f, 2.0f } }
	   
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
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    uint32_t width, height; // 1. uvStepSizeの算出
    gTexture.GetDimensions(width, height);
    float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));
    
   
    float32_t2 difference = float32_t2(0.0f, 0.0f); // 縦横それぞれの畳み込みの結果を格納する
    // 色を輝度に変換して、畳み込みを行っていく。微分Filter用のkernelになっているので、やること自体は今までの畳み込みと同じ
    for (int32_t x = 0; x < KenelSize; ++x)
    {
        for (int32_t y = 0; y < KenelSize; ++y)
        {
            // 3. 現在のtexcoordを算出
            float32_t2 texcoord = input.texcoord + kIndex3x3[x][y] * uvStepSize;
            
            float32_t ndcDepth = gDepthTexture.Sample(gSamplerPoint, texcoord);
            // NDC -> View。P^{-1}においてxとyはzwに影響を与えないので何でもよい。なので、わざわざ行列を渡さなくてもよい
            // gMaterial.projectionInverseはCBufferを使って渡しておくこと
            float32_t4 viewSpace = mul(float32_t4(0.0f, 0.0f, ndcDepth, 1.0f), gMaterial.projectionInverse);
            float32_t viewZ = viewSpace.z * rcp(viewSpace.w); // 同時座標系からデカルト座標系へ変換
            difference.x += viewZ * kPrewittHorizontalKernel[x][y];
            difference.y += viewZ * kPrewittVerticalKernel[x][y];

        }
    }
    
    
    // 変化の長さをウェイトとして合成。ウェイトの決定方法も色々と考えられる。例えばdifference.xだけ使えば横方向のエッジが検出される
    float32_t weight = length(difference);
    // 差が小さい過ぎてわかりずらいので適当に6倍している。CBufferで調整パラメータとして送ったりすると良い
    
    //if (weight >= 30.0f || (weight >= 0.5f && weight <= 1.0f))
    //{
    //    weight = saturate(weight);
    //}
    //else
    //{
    //    weight = 0.0f;

    //}
    float32_t ndcDepth = gDepthTexture.Sample(gSamplerPoint, input.texcoord);
    float32_t4 viewSpace = mul(float32_t4(0.0f, 0.0f, ndcDepth, 1.0f), gMaterial.projectionInverse);
    float32_t viewZ = viewSpace.z * rcp(viewSpace.w); // 同時座標系からデカルト座標系へ変換
    float32_t ndcDepth1 = gDepthTexture.Sample(gSamplerPoint, input.texcoord);
    if (length(difference) >= 0.5 && viewZ < 100)
    {
        weight = saturate(weight);
    }
    else
    {
        weight = 0.0f;

    }
    
    PixelShaderOutput output;
    // weightが大きいほど暗く表示するようにしている。最もシンプルな合成方法
    output.color.rgb = (1.0f - weight) * gTexture.Sample(gSampler, input.texcoord).rgb;
    output.color.a = 1.0f;
  
    return output;

};