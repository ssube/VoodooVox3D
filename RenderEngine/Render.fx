
texture BaseTexture;
sampler2D DiffuseSampler = sampler_state
{
	texture = BaseTexture; 
};

float4x4 ModelViewProj;

struct VertexIn
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float3 TexCoord : TEXCOORD;
	float4 Color : COLOR;
};

struct VertexOut
{
	float4 Position : POSITION;
	float2 TexCoord : TEXCOORD;
	float4 Color : COLOR;
};

void vstage(in VertexIn iv, out VertexOut ov)
{
	ov.Position = mul(ModelViewProj, float4(iv.Position, 1.0f));
	ov.TexCoord = iv.TexCoord.xy;
	ov.Color = iv.Color;
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
