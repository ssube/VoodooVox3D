
texture BaseTexture : TEXTURE_BASE;
sampler2D DiffuseSampler = sampler_state
{
	texture = BaseTexture; 
};

float4x4 ModelViewProj : MATRIX_MVP;

struct VertexIn
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float3 TexCoord : TEXCOORD0;
	float4 Color : TEXCOORD1;
};

struct VertexOut
{
	float4 Position : POSITION;
	float2 TexCoord : TEXCOORD0;
	float4 Color : COLOR0;
};

void vstage(in VertexIn iv, out VertexOut ov)
{
	ov.Position = mul(iv.Position, ModelViewProj);
	ov.TexCoord = iv.TexCoord.xy;
	ov.Color.xyz = floor(iv.Color.xyz + 0.1f);
	ov.Color.w = 1.0f;
}

void pstage(in VertexOut iv, out float4 color : COLOR)
{
	color = tex2D(DiffuseSampler, iv.TexCoord) * iv.Color;
}

technique Base_NoLight
{
	pass p0
	{
		VertexShader = compile vs_2_0 vstage();
		PixelShader  = compile ps_2_0 pstage();
	}
}
