Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PSInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

// 定数バッファの代わりに定数を直接シェーダ内で定義
static const float hue = 0.3f; // 色相調整値
static const float saturation = 0.5f; // 彩度調整値
static const float value = 0.6f; // 明度調整値

float WrapValue(float value, float minRange, float maxRange)
{
    float range = maxRange - minRange;
    float modValue = fmod(value - minRange, range);
    if (modValue < 0)
    {
        modValue += range;
    }
    return minRange + modValue;
}

float3 RGBToHSV(float3 rgb)
{
    float maxC = max(rgb.r, max(rgb.g, rgb.b));
    float minC = min(rgb.r, min(rgb.g, rgb.b));
    float delta = maxC - minC;

    float h = 0.0;
    if (delta != 0.0)
    {
        if (maxC == rgb.r)
        {
            h = (rgb.g - rgb.b) / delta + (rgb.g < rgb.b ? 6.0 : 0.0);
        }
        else if (maxC == rgb.g)
        {
            h = (rgb.b - rgb.r) / delta + 2.0;
        }
        else
        {
            h = (rgb.r - rgb.g) / delta + 4.0;
        }
        h /= 6.0;
    }

    float s = maxC == 0.0 ? 0.0 : delta / maxC;
    float v = maxC;

    return float3(h, s, v);
}

float3 HSVToRGB(float3 hsv)
{
    float h = hsv.x * 6.0;
    float s = hsv.y;
    float v = hsv.z;

    int i = int(h);
    float f = h - i;
    float p = v * (1.0 - s);
    float q = v * (1.0 - s * f);
    float t = v * (1.0 - s * (1.0 - f));

    float3 rgb;
    switch (i % 6)
    {
        case 0:
            rgb = float3(v, t, p);
            break;
        case 1:
            rgb = float3(q, v, p);
            break;
        case 2:
            rgb = float3(p, v, t);
            break;
        case 3:
            rgb = float3(p, q, v);
            break;
        case 4:
            rgb = float3(t, p, v);
            break;
        case 5:
            rgb = float3(v, p, q);
            break;
    }

    return rgb;
}

float4 main(PSInput input) : SV_TARGET
{
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    float3 hsv = RGBToHSV(textureColor.rgb);

    // HSVを調整
    hsv.x = WrapValue(hsv.x + hue, 0.0f, 1.0f);
    hsv.y = saturate(hsv.y + saturation);
    hsv.z = saturate(hsv.z + value);

    float3 rgb = HSVToRGB(hsv);
    return float4(rgb, textureColor.a);
}
