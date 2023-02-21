


Texture2D texDiffuse : register(t0);
Texture2D texNormal : register(t1);
TextureCube texCube : register(t2);

SamplerState texSampler : register(s0);
SamplerState cubeSampler : register(s1);

struct PSIn
{
    float4 Pos : SV_Position;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
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

    
    //phong & textures
    
    float3x3 TBN = transpose(float3x3(input.Tangent, input.Binormal, input.Normal));
    
    //float4 textureNoral = texNormal.Sample(texSampler, input.TexCoord) * 2 - 1;
    float4 textureNormal = texNormal.Sample(texSampler, input.TexCoord);
    float3 N = mul(TBN, textureNormal.xyz);
    

    
    float4 Ia = Ambient;

    float4 L = float4(normalize(LightPos.xyz - input.WorldPos), 0);
    float4 Id = max(Diffuse * dot(L.xyz, N), 0);
    //float4 Id = max(Diffuse * dot(L.xyz, input.Normal), 0);

    float3 R = normalize(reflect(-L.xyz, N.xyz));
    //float3 R = normalize(reflect(-L.xyz, input.Normal.xyz));
    float4 V = float4(normalize(CamPos.xyz - input.WorldPos), 0);
	
    float4 Is = max(Specular * pow(max(dot(V.xyz, R), 0), Alpha), 0);

    //cubemap
    float3 cubeReflection = reflect(V.xyz, N);
    //float4 cubeMap = texCube.Sample(cubeSampler, cubeReflection);
    float4 cubeMap = texCube.Sample(texSampler, -V.xyz);
    return texCube.Sample(cubeSampler, V.xyz);
    
    Id = Id * cubeMap;
    Is = Is * cubeMap;
    
    float4 I = Ia + Id + Is;
    //float4 I = Ia * cubeMap.x + Id * cubeMap.y + Is * cubeMap.z;
    //float4 I = Ia + cubeMap;
    I.a = 1;
    //return I;
    //return float4(input.Tangent, 1);
    //return float4(input.Tangent * 0.5 + 0.5, 1);
    //return float4(input.Binormal * 0.5 + 0.5, 1);
    //return float4(N *  0.5 + 0.5, 1);
    
    
    return texDiffuse.Sample(texSampler, input.TexCoord) * I;


    
	// Debug shading #1: map and return normal as a color, i.e. from [-1,1]->[0,1] per component
	// The 4:th component is opacity and should be = 1
	//return float4(input.Normal * 0.5 + 0.5, 1);

	//Debug shading #2: map and return texture coordinates as a color (blue = 0)
	//return float4(input.TexCoord, 0, 1);

	//Blue
	//return float4(1, 0, 0, 1);

}

