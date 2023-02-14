


Texture2D texDiffuse : register(t0);

SamplerState texSampler : register(s0);

struct PSIn
{
    float4 Pos : SV_Position;
    float3 Normal : NORMAL;
    float2 TexCoord : TEX;
    float3 WorldPos : WORLDPOSITION;
};

cbuffer LightCamBuffer : register(b0)
{
    float4 LightPos;
    float4 CamPos;
};

cbuffer MaterialBuffer : register(b1)
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float Alpha;
    float3 pad;
};

//-----------------------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------------------

float4 PS_main(PSIn input) : SV_Target
{
    
    float4 Ia = Ambient;

    float4 L = float4(normalize(LightPos.xyz - input.WorldPos), 0);
    float4 Id = max(Diffuse * dot(L.xyz, input.Normal), 0);

    float3 R = normalize(reflect(-L.xyz, input.Normal));
    float4 V = float4(normalize(CamPos.xyz - input.WorldPos), 0);
    //float3 R = normalize(reflect(V.xyz, input.Normal));
	
    float4 Is = max(Specular * pow(max(dot(V.xyz, R), 0), Alpha), 0);
    //float4 Is = max(Specular * pow(abs(dot(V.xyz, R)), Alpha), 0);

    float4 I = Ia + Id + Is;
    //return I;   

    return texDiffuse.Sample(texSampler, input.TexCoord) * I;

	// Debug shading #1: map and return normal as a color, i.e. from [-1,1]->[0,1] per component
	// The 4:th component is opacity and should be = 1
	//return float4(input.Normal * 0.5 + 0.5, 1);

	//Debug shading #2: map and return texture coordinates as a color (blue = 0)
	//return float4(input.TexCoord, 0, 1);

	//Blue
	//return float4(1, 0, 0, 1);

}

