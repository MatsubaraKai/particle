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


float32_t rand2dTo1d(float32_t2 input, float32_t2 dotDir = float32_t2(gMaterial.projectionInverse.x, gMaterial.projectionInverse.y))
{
    float32_t2 smallValue = sin(input);
    float32_t random = dot(smallValue, dotDir);
    random = frac(sin(random) * 143758.5453f);
    return random;
}

float32_t rand(float32_t3 vec)
{
     //make value smaller to avoid artefacts
    float32_t3 smallValue = sin(vec);
    //get scalar value from 3d vector
    float32_t random = dot(smallValue, float32_t3(12.9898f, 78.233f, 37.719f));
    //make value more random by making it bigger and then taking teh factional part
    random = frac(sin(random) * 143758.5453f);
    return random;
}

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
	// 乱数生成。引数にtexcoordを和刺している
    float32_t random = rand2dTo1d(float32_t2(input.texcoord.x, input.texcoord.y));
	// 色にする
    output.color = float32_t4(random, random, random, 1.0f);
    return output;

};