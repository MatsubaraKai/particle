#include "Fullscreen.hlsli"

struct Material
{
    float32_t hue;
    float32_t saturation;
    float32_t value;
    
};

float32_t WrapValue(float32_t value, float32_t minRange, float32_t maxRange)
{
    float32_t range = maxRange - minRange;
    float32_t modValue = fmod(value - minRange, range);
    if (modValue < 0)
    {
        modValue += range;
    }
    return minRange + modValue;
}

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<Material> gMaterial : register(b0);
struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

float32_t3 RGBToHSV(float32_t3 rgb)
{
    
    float32_t max = rgb.x;
    float32_t min = rgb.x;
    if (max < rgb.y)
    {
        max = rgb.y;
    }
    if (max < rgb.z)
    {
        max = rgb.z;
    }
    if (min > rgb.y)
    {
        min = rgb.y;
    }
    if (min > rgb.z)
    {
        min = rgb.z;
    }
    
    float32_t h = max - min;
    
    if (h > 0.0f)
    {
        if (max == rgb.x)
        {
            h = (rgb.y - rgb.z) / h;
            if (h < 0.0f)
            {
                h += 6.0f;
            }
        }
        else if (max == rgb.y)
        {
            h = 2.0f + (rgb.z - rgb.x) / h;
        }
        else if (max == rgb.z)
        {
            h = 4.0f + (rgb.x - rgb.y) / h;
        }
    }
    
    h /= 6.0f;
    float32_t s = (max - min);
    if (max != 0.0f)
    {
        s /= max;
    }
    
    float32_t v = max;
    
    float32_t3 returnHsv = { h, s, v };
    return returnHsv;
	
}

float32_t3 HSVToRGB(float32_t3 hsv)
{

    float32_t r = hsv.z;
    float32_t g = hsv.z;
    float32_t b = hsv.z;
    if (hsv.y > 0.0f)
    {
        hsv.x *= 6.0f;
        int i = (int) hsv.x;
        float f = hsv.x - (float) i;
        switch (i)
        {
            default:
            case 0:
                g *= 1 - hsv.y * (1 - f);
                b *= 1 - hsv.y;
                break;
            case 1:
                r *= 1 - hsv.y * f;
                b *= 1 - hsv.y;
                break;
            case 2:
                r *= 1 - hsv.y;
                b *= 1 - hsv.y * (1 - f);
                break;
            case 3:
                r *= 1 - hsv.y;
                g *= 1 - hsv.y * f;
                break;
            case 4:
                r *= 1 - hsv.y * (1 - f);
                g *= 1 - hsv.y;
                break;
            case 5:
                g *= 1 - hsv.y;
                b *= 1 - hsv.y * f;
                break;
        }
    }
    float32_t3 rgb = { r, g, b };
    return rgb;
	
}

PixelShaderOutput main(VertexShaderOutput input)
{
    float32_t4 textureColor =
    gTexture.Sample(gSampler, input.texcoord);
    float32_t3 hsv = RGBToHSV(textureColor.rgb);
    
   
    
    hsv.x += gMaterial.hue;
    hsv.y += gMaterial.saturation;
    hsv.z += gMaterial.value;
    
    hsv.x = WrapValue(hsv.x, 0.0f, 1.0f);
    hsv.y = saturate(hsv.y);
    hsv.z = saturate(hsv.z);
    float32_t3 rgb = HSVToRGB(hsv);
    PixelShaderOutput output;
    output.color.rgb = rgb;
    output.color.a = textureColor.a;
    return output;

};