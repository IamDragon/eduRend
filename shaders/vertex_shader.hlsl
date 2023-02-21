
cbuffer TransformationBuffer : register(b0)
{
    matrix ModelToWorldMatrix;
    matrix WorldToViewMatrix;
    matrix ProjectionMatrix;
};

struct VSIn
{
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
    float2 TexCoord : TEX;
    //float3 WorldPos : WORLDPOSITION;
};

struct PSIn
{
    float4 Pos : SV_Position;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
    float2 TexCoord : TEX;
    float3 WorldPos : WORLDPOSITION;
};

//-----------------------------------------------------------------------------------------
// Vertex Shader
//-----------------------------------------------------------------------------------------

PSIn VS_main(VSIn input)
{
    PSIn output = (PSIn) 0;
	
	// Model->View transformation
    matrix MV = mul(WorldToViewMatrix, ModelToWorldMatrix);

	// Model->View->Projection (clip space) transformation
	// SV_Position expects the output position to be in clip space
    matrix MVP = mul(ProjectionMatrix, MV);
    
    //float3x3 TBN = transpose(float3x3(input.Tangent, input.Binormal, input.Normal));
	
    float texScale = 1;
    
	// Perform transformations and send to output
    output.Pos = mul(MVP, float4(input.Pos, 1));
    output.Normal = normalize(mul(ModelToWorldMatrix, float4(input.Normal, 0)).xyz);
    //output.Tangent = mul(ModelToWorldMatrix, float4(mul(TBN, input.Tangent),0)).xyz; // tangent in worldspace
    //output.Binormal = mul(ModelToWorldMatrix, float4(mul(TBN, input.Binormal), 0)).xyz; //binormal in worldspace
    output.Tangent = normalize(mul(ModelToWorldMatrix, float4(input.Tangent, 0)).xyz);
    output.Binormal = normalize(mul(ModelToWorldMatrix, float4(input.Binormal, 0)).xyz);
    output.TexCoord = texScale * input.TexCoord;
    output.WorldPos = mul(ModelToWorldMatrix, float4(input.Pos, 0)).xyz;
		
    
    return output;
}