#include "Fullscreen.hlsli"
struct Material
{
    float32_t projectionInverse;
};

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

static const float32_t PI = 3.14159265f;

float gauss(float x, float y, float sigma)
{
    float exponet = -(x * x + y * y) * rcp(2.0f * sigma * sigma);
    float denominator = 2.0f * PI * sigma * sigma;
    return exp(exponet) * rcp(denominator);

}

static const int32_t KenelSize = 5;

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




struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    uint32_t width, height; // 1. uvStepSizeの算出
    gTexture.GetDimensions(width, height);
    float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));
    
    // kernelを求める weightは後で使う
    float32_t weight = 0.0f;
    float32_t kernel5x5[5][5];
    for (int32_t x = 0; x < KenelSize; ++x)
    {
        for (int32_t y = 0; y < KenelSize; ++y)
        {
            kernel5x5[x][y] = gauss(kIndex5x5[x][y].x, kIndex5x5[x][y].y, gMaterial.projectionInverse);
            weight += kernel5x5[x][y];
        }
    }
    PixelShaderOutput output;
    output.color.rgb = float32_t3(0.0f, 0.0f, 0.0f);
    output.color.a = 1.0f;
    
    for (int32_t x1 = 0; x1 < KenelSize; ++x1)
    {
        for (int32_t y1 = 0; y1 < KenelSize; ++y1)
        {
            // 3. 現在のtexcoordを算出
            float32_t2 texcoord = input.texcoord + kIndex5x5[x1][y1] * uvStepSize;
            // 4. 色に1/9掛けて足す
            float32_t3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
            output.color.rgb += fetchColor * kernel5x5[x1][y1];
            

        }
        
    }
    
    // 畳み込み後の値を正規化する。本来gauss関数は全体を合計すると（積分）になるように設計されている。しかし、無限の範囲は足せないので、kernel値
    // の合計であるweughtは1に満たない。なので、合計が1になるように逆数を掛けて全体を底上げして調整する
  
    output.color.rgb *= rcp(weight);
    //output.color.rgb *= normalize(weight);
    
    return output;

};