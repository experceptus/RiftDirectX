struct BoxInit
{
	ID3D11VertexShader      * D3DVert;
	ID3D11PixelShader       * D3DPix;
	//	Texture                 * Tex;
	ID3D11InputLayout       * InputLayout;
	UINT                      VertexSize;

	ID3D11Buffer * VertBuffer;
	ID3D11Buffer * IndexBuffer;

	ID3DBlob* VS_Buffer;
	ID3DBlob* PS_Buffer;

	ID3D11Buffer * D3DBuffer;

	HRESULT hr;


	ID3D11Buffer* cbPerObjectBuffer;

	struct cbPerObject
	{
		//DirectX::XMMATRIX  WVP;
		Matrix4f WVP;
	};

	cbPerObject cbPerObj;

	struct Vertex	//Overloaded Vertex Structure
	{
		Vertex(){}
		Vertex(float x, float y, float z,
			float cr, float cg, float cb, float ca)
			: pos(x, y, z), color(cr, cg, cb, ca){}

		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT4 color;
	};

	BoxInit(bool isHot)
	{

		D3D11_INPUT_ELEMENT_DESC defaultVertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = ARRAYSIZE(defaultVertexDesc);


		char* defaultVertexShaderSrc =
			"cbuffer cbPerObject"
			"{"
			"	float4x4 WVP;"
			"};"
			"struct VS_OUTPUT"
			"{"
			"	float4 Pos : SV_POSITION;"
			"	float4 Color : COLOR;"
			"};"

			"VS_OUTPUT VS(float4 inPos : POSITION, float4 inColor : COLOR)"
			"{"
			"   VS_OUTPUT output;"
			"	output.Pos = mul(inPos, WVP);"
			"	output.Color = inColor;"
			"	return output;"
			"}";

		char* defaultPixelShaderSrc =
			"struct VS_OUTPUT"
			"{"
			"	float4 Pos : SV_POSITION;"
			"	float4 Color : COLOR;"
			"};"
			"float4 PS(VS_OUTPUT input) : SV_TARGET"
			"{return input.Color;}";



		////////////////////////////////
		/*
		// Create sampler state
		D3D11_SAMPLER_DESC ss; memset(&ss, 0, sizeof(ss));
		ss.AddressU = ss.AddressV = ss.AddressW = flags & MAT_WRAP ? D3D11_TEXTURE_ADDRESS_WRAP : D3D11_TEXTURE_ADDRESS_BORDER;
		ss.Filter = D3D11_FILTER_ANISOTROPIC;
		ss.MaxAnisotropy = 8;
		ss.MaxLOD = 15;
		DIRECTX.Device->CreateSamplerState(&ss, &SamplerState);

		// Create rasterizer
		D3D11_RASTERIZER_DESC rs; memset(&rs, 0, sizeof(rs));
		rs.AntialiasedLineEnable = rs.DepthClipEnable = true;
		rs.CullMode = flags & MAT_NOCULL ? D3D11_CULL_NONE : D3D11_CULL_BACK;
		rs.FillMode = flags & MAT_WIRE ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
		DIRECTX.Device->CreateRasterizerState(&rs, &Rasterizer);

		// Create depth state
		D3D11_DEPTH_STENCIL_DESC dss;
		memset(&dss, 0, sizeof(dss));
		dss.DepthEnable = true;
		dss.DepthFunc = flags & MAT_ZALWAYS ? D3D11_COMPARISON_ALWAYS : D3D11_COMPARISON_LESS;
		dss.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		DIRECTX.Device->CreateDepthStencilState(&dss, &DepthState);

		*/
		/////////////////////////////////
		//Create the Shader Objects



		//Set Vertex and Pixel Shaders
		//Render - DIRECTX.Context->VSSetShader(D3DVert, 0, 0);
		//Render - DIRECTX.Context->PSSetShader(D3DPix, 0, 0);

		//Create the vertex buffer
		Vertex cubeVertices[] =
		{
			Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
			Vertex(-1.0f, +1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f),
			Vertex(+1.0f, +1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f),
			Vertex(+1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 1.0f),
			Vertex(-1.0f, -1.0f, +1.0f, 0.0f, 1.0f, 1.0f, 1.0f),
			Vertex(-1.0f, +1.0f, +1.0f, 1.0f, 1.0f, 1.0f, 1.0f),
			Vertex(+1.0f, +1.0f, +1.0f, 1.0f, 0.0f, 1.0f, 1.0f),
			Vertex(+1.0f, -1.0f, +1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
		};

		DWORD cubeIndices[] = {
			// front face
			0, 1, 2,
			0, 2, 3,

			// back face
			4, 6, 5,
			4, 7, 6,

			// left face
			4, 5, 1,
			4, 1, 0,

			// right face
			3, 2, 6,
			3, 6, 7,

			// top face
			1, 5, 6,
			1, 6, 2,

			// bottom face
			4, 0, 3,
			4, 3, 7
		};

		
		// Create vertex shader

		D3DCompile(defaultVertexShaderSrc, strlen(defaultVertexShaderSrc), 0, 0, 0, "VS", "vs_4_0", 0, 0, &VS_Buffer, 0);
		hr = DIRECTX.Device->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &D3DVert);

		// Create input layout
		DIRECTX.Device->CreateInputLayout(defaultVertexDesc, numElements,
			VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), &InputLayout);


		// Create pixel shader
		D3DCompile(defaultPixelShaderSrc, strlen(defaultPixelShaderSrc), 0, 0, 0, "PS", "ps_4_0", 0, 0, &PS_Buffer, 0);
		hr = DIRECTX.Device->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &D3DPix);

		

		// describe the vertex buffer to create
		D3D11_BUFFER_DESC desc;   memset(&desc, 0, sizeof(desc));
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = (unsigned)sizeof(cubeVertices);

		// describe the vertex data that will initialize the buffer
		D3D11_SUBRESOURCE_DATA sr;
		sr.pSysMem = cubeVertices;
		sr.SysMemPitch = 0;
		sr.SysMemSlicePitch = 0;
		DIRECTX.Device->CreateBuffer(&desc, &sr, &VertBuffer);

		// describe the index buffer to create
		D3D11_BUFFER_DESC index;   memset(&index, 0, sizeof(index));
		index.Usage = D3D11_USAGE_DYNAMIC;
		index.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		index.BindFlags = D3D11_BIND_INDEX_BUFFER;
		index.ByteWidth = (unsigned)sizeof(cubeIndices);

		// describe the index data that will initialize the buffer
		D3D11_SUBRESOURCE_DATA sr2;
		sr2.pSysMem = cubeIndices;
		sr2.SysMemPitch = 0;
		sr2.SysMemSlicePitch = 0;
		DIRECTX.Device->CreateBuffer(&index, &sr2, &IndexBuffer);

		// create the constants buffer

		D3D11_BUFFER_DESC cb;   
		memset(&cb, 0, sizeof(cb));
		cb.Usage = D3D11_USAGE_DYNAMIC;
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = sizeof(cbPerObject);

		DIRECTX.Device->CreateBuffer(&cb, NULL, &cbPerObjectBuffer);
	
		// Buffer for shader constants
	
		// Initial update with no data 
		DIRECTX.Context->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
		/*
		DIRECTX.Context->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);
	
		
	


		*/

	} // end Material CTOR




};  // end Material

struct SingleBox
{
	Vector3f     Pos;
	Quatf        Rot;
	BoxInit     *   Fill;




	SingleBox(int one) {
		Fill = new BoxInit(true);
	

		std::cout << " In SingleBox CTOR" << std::endl;;
		;

	};



	void render2() {

	}

	void UpdateScene()
	{




	}

	void Render2(Matrix4f projView, float R, float G, float B, float A, bool standardUniforms)
	{
		
		Matrix4f modelMat = Matrix4f::Translation(Pos) * Matrix4f(Rot);
		Matrix4f mat = (projView * modelMat).Transposed();
		float col[] = { R, G, B, A };
		if (standardUniforms) memcpy(DIRECTX.UniformData + 0, &mat, 64); // ProjView
		if (standardUniforms) memcpy(DIRECTX.UniformData + 64, &col, 16); // MasterCol
		D3D11_MAPPED_SUBRESOURCE map;
		DIRECTX.Context->Map(DIRECTX.UniformBufferGen->D3DBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
		memcpy(map.pData, &DIRECTX.UniformData, UNIFORM_DATA_SIZE);
		DIRECTX.Context->Unmap(DIRECTX.UniformBufferGen->D3DBuffer, 0);
		//Set the Input Layout
		DIRECTX.Context->IASetInputLayout(Fill->InputLayout);
		DIRECTX.Context->IASetIndexBuffer(Fill->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//Set the vertex buffer
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		DIRECTX.Context->IASetVertexBuffers(0, 1, &Fill->VertBuffer, &stride, &offset);
		//Set Primitive Topology
		DIRECTX.Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);




		

		//DIRECTX.Context->VSSetConstantBuffers(0, 1, &Fill->cbPerObjectBuffer);

		//DIRECTX.Context->VSSetShader(Fill->D3DVert, NULL, 0);
		//DIRECTX.Context->PSSetShader(Fill->D3DPix, NULL, 0);


		//WVP = cube1World * camView * camProjection;
		//Fill->cbPerObj.WVP = DirectX::XMMatrixTranspose(projView * modelMat);

		// Turned this into a Matrix4f. Might want to turn it back. 
		Fill->cbPerObj.WVP = (projView * modelMat).Transposed();

		// Try these to next
		// https://msdn.microsoft.com/en-us/library/windows/desktop/ff476486%28v=vs.85%29.aspx

		//I'm not updating this correctly. These are the numbers for creation, not update. The last two must
		// be something like stride and offset. Look a the link above. 
		DIRECTX.Context->UpdateSubresource(Fill->cbPerObjectBuffer, 0, NULL, &Fill->cbPerObj, 0, 0);

		// Always kills scene
		//DIRECTX.Context->VSSetConstantBuffers(0, 1, &Fill->cbPerObjectBuffer);

		//Draw the first cube
		DIRECTX.Context->DrawIndexed(36, 0, 0);

		//WVP = cube2World * camView * camProjection;
		//cbPerObj.WVP = XMMatrixTranspose(WVP);
		//d3d11DevCon->UpdateSubresource(cbPerObjectBuffer, 0, NULL, &cbPerObj, 0, 0);
		//d3d11DevCon->VSSetConstantBuffers(0, 1, &cbPerObjectBuffer);

		//Draw the second cube
		//DIRECTX.Context->DrawIndexed(36, 0, 0);
	
	}



};
