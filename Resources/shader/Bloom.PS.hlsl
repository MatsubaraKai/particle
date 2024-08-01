#include "Fullscreen.hlsli"

struct Material
{
    float32_t deviation;
    float32_t luminance;
    float32_t brightness;
    int32_t kernel;
};

ConstantBuffer<Material> gMaterial : register(b0);

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

static const int32_t KenelSizeMax = 2 * 5 + 1;

static const float32_t PI = 3.14159265f;
float gauss(float x, float y, float sigma)
{
    float exponet = -(x * x + y * y) * rcp(2.0f * sigma * sigma);
    float denominator = 2.0f * PI * sigma * sigma;
    return exp(exponet) * rcp(denominator);

}



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

     // kernelを求める weightは後で使う
    float32_t weight = 0.0f;
    float32_t kernel[KenelSizeMax][KenelSizeMax];
    float32_t2 kIndex[KenelSizeMax][KenelSizeMax];
  
    float32_t3 bloomGauss = float32_t3(0.0f, 0.0f, 0.0f);
    for (int32_t x = 0; x < KenelSizeMax; ++x)
    {
        for (int32_t y = 0; y < KenelSizeMax; ++y)
        {
            kIndex[x][y].x = 0.0f - ((KenelSizeMax - 1) / 2 - x);
            kIndex[x][y].y = 0.0f - ((KenelSizeMax - 1) / 2 - y);
            kernel[x][y] = gauss(kIndex[x][y].x, kIndex[x][y].y, gMaterial.deviation);
            weight += kernel[x][y];
            // 3. 現在のtexcoordを算出
            float32_t2 texcoord = input.texcoord + kIndex[x][y] * uvStepSize;
            float32_t3 bloom = gTexture.Sample(gSampler, texcoord).rgb;
            float32_t luminance = Luminance(bloom);

            if (luminance >= gMaterial.luminance)
            {
                bloom = bloom
                * gMaterial.brightness;
            }
            else
            {
                bloom = 0.0f;
            }
            
            // weightが大きいほど暗く表示するようにしている。最もシンプルな合成方法
            bloomGauss += bloom * kernel[x][y];
           

        }
        
    }
    bloomGauss *= rcp(weight);

    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);
    output.color.rgb += bloomGauss;
    output.color.a = 1.0f;
  
    return output;
  
};