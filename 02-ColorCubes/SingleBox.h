struct BoxInit
{
	ID3D11VertexShader      * D3DVert;
	ID3D11PixelShader       * D3DPix;
	//	Texture                 * Tex;
	ID3D11InputLayout       * InputLayout;
	UINT                      VertexSize;

	ID3D11Buffer * VertBuffer;
	ID3D11Buffer * IndexBuffer;

	ID3D11Buffer * D3DBuffer;

	HRESULT hr;
	//	enum { MAT_WRAP = 1, MAT_WIRE = 2, MAT_ZALWAYS = 4, MAT_NOCULL = 8 };


	//Material()  {}


	BoxInit(bool isHot)
	{
		D3D11_INPUT_ELEMENT_DESC defaultVertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		// Used to send per-vertex data to the vertex shader.
		struct VertexPositionColor
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT3 color;
		};

		ID3D11VertexShader  * m_vertexShader;
		ID3D11PixelShader * m_pixelShader;
		std::ifstream vs_stream;
		std::ifstream ps_stream;
		size_t vs_size;
		size_t ps_size;
		char* vs_data;
		char* ps_data;
		int Length;
		bool hr = false;
		// read the vertex shader
		char* defaultVertexShaderSrc =
			"cbuffer ModelViewProjectionConstantBuffer : register(b0)"
			"{matrix model;matrix view;matrix projection;};"
			"struct VertexShaderInput"
			"{float3 pos : POSITION;float3 color : COLOR0;};"
			"struct PixelShaderInput"
			"{float4 pos : SV_POSITION;float3 color : COLOR0;};"
			"PixelShaderInput main(VertexShaderInput input)"
			"{PixelShaderInput output;float4 pos = float4(input.pos, 1.0f);"
			"pos = mul(pos, model);pos = mul(pos, view);pos = mul(pos, projection);"
			"output.pos = pos;output.color = input.color;"
			"return output;}";


		char* defaultPixelShaderSrc =
			"struct PixelShaderInput"
			"{float4 pos : SV_POSITION;float3 color : COLOR0;};"
			"float4 main(PixelShaderInput input) : SV_TARGET"
			"{return float4(input.color, 1.0f);}";

		// Create vertex shader
		ID3DBlob * blobData;
		D3DCompile(defaultVertexShaderSrc, strlen(defaultVertexShaderSrc), 0, 0, 0, "main", "vs_4_0", 0, 0, &blobData, 0);
		DIRECTX.Device->CreateVertexShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), NULL, &D3DVert);

		// Create input layout
		DIRECTX.Device->CreateInputLayout(defaultVertexDesc, 2,
			blobData->GetBufferPointer(), blobData->GetBufferSize(), &InputLayout);
		blobData->Release();

		// Create pixel shader
		D3DCompile(defaultPixelShaderSrc, strlen(defaultPixelShaderSrc), 0, 0, 0, "main", "ps_4_0", 0, 0, &blobData, 0);
		DIRECTX.Device->CreatePixelShader(blobData->GetBufferPointer(), blobData->GetBufferSize(), NULL, &D3DPix);
		

		VertexSize = 24;

		blobData->Release();
		/*
		vs_stream.open("testVS.cso", std::ifstream::in | std::ifstream::binary);

		if (vs_stream.is_open())
		{
		vs_stream.seekg(0, std::ios::end);
		vs_size = size_t(vs_stream.tellg());
		vs_data = new char[vs_size];
		vs_stream.seekg(0, std::ios::beg);
		vs_stream.read(&vs_data[0], vs_size);
		vs_stream.close();
		hr = DIRECTX.Device->CreateVertexShader(&vs_data, vs_size, nullptr, &D3DVert);
		D3D_FEATURE_LEVEL ahw = DIRECTX.Device->GetFeatureLevel();
		if (!(hr))
		{
		std::cout << "Vertex Shader loader failed" << std::endl;
		}
		}
		else {
		std::cout << " Could not open vertex .cso file " << std::endl;
		}
		// use the top variable. Just doing this to match with the other names
		VertexSize = vs_size;
		hr = false;

		// read the pixel shader
		ps_stream.open("PixelShader.cso", std::ifstream::in | std::ifstream::binary);
		if (ps_stream.good())
		{
		ps_stream.seekg(0, std::ios::end);
		ps_size = size_t(ps_stream.tellg());
		ps_data = new char[ps_size];
		ps_stream.seekg(0, std::ios::beg);
		ps_stream.read(&ps_data[0], ps_size);
		ps_stream.close();
		hr = DIRECTX.Device->CreatePixelShader(&ps_data, ps_size, 0, &D3DPix);
		if (!(hr))
		{
		std::cout << "Vertex Shader loader failed" << std::endl;
		}
		}
		else {
		std::cout << " Could not open vertex .cso file " << std::endl;
		}

		hr = false;

		// Create input layout
		hr = DIRECTX.Device->CreateInputLayout(defaultVertexDesc, 3,
		vs_data, vs_size, &InputLayout);

		*/

		// Load mesh vertices. Each vertex has a position and a color.
		static const VertexPositionColor cubeVertices[] =
		{
			{ DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f) },
			{ DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f) },
			{ DirectX::XMFLOAT3(-0.5f, 0.5f, -0.5f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f) },
			{ DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(0.0f, 1.0f, 1.0f) },
			{ DirectX::XMFLOAT3(0.5f, -0.5f, -0.5f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f) },
			{ DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f), DirectX::XMFLOAT3(1.0f, 0.0f, 1.0f) },
			{ DirectX::XMFLOAT3(0.5f, 0.5f, -0.5f), DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f) },
			{ DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f) },
		};

		// define the indices
		static const unsigned short cubeIndices[] =
		{
			0, 2, 1, // -x
			1, 2, 3,

			4, 5, 6, // +x
			5, 7, 6,

			0, 1, 5, // -y
			0, 5, 4,

			2, 6, 7, // +y
			2, 7, 3,

			0, 4, 6, // -z
			0, 6, 2,

			1, 3, 7, // +z
			1, 7, 5,
		};

		// create a vertex buffer. Just like Vertex array in OpenGL



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

	} // end Material CTOR




};  // end Material

struct SingleBox
{
	Vector3f     Pos;
	Quatf        Rot;
	BoxInit     *   Fill;
//	DXVertexBuffer * VertexBuffer;
//	DXIndexBuffer  * IndexBuffer;
//	int              NumIndices;




	SingleBox(int one) {
		Fill = new BoxInit(true);
	

		std::cout << " In SingleBox CTOR" << std::endl;;
		;

	};

	//TextureBuffer * generated_texture = new TextureBuffer(nullptr, false, false, Sizei(256, 256), 4, (unsigned char *)tex_pixels, 1);
	//grid_material[k] = new ShaderFill(vshader, fshader, generated_texture);




	// create a triangle out of vertices


	//


	void render2() {

	}

	void Render(Matrix4f projView, float R, float G, float B, float A, bool standardUniforms)
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


		DIRECTX.Context->IASetInputLayout(Fill->InputLayout);
		DIRECTX.Context->IASetIndexBuffer(Fill->IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		UINT offset = 0;
		DIRECTX.Context->IASetVertexBuffers(0, 1, &Fill->VertBuffer, &Fill->VertexSize, &offset);
		DIRECTX.Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DIRECTX.Context->VSSetShader(Fill->D3DVert, NULL, 0);
		DIRECTX.Context->PSSetShader(Fill->D3DPix, NULL, 0);
		//DIRECTX.Context->PSSetSamplers(0, 1, &Fill->SamplerState);
		//DIRECTX.Context->RSSetState(Fill->Rasterizer);
		//DIRECTX.Context->OMSetDepthStencilState(Fill->DepthState, 0);
		//DIRECTX.Context->PSSetShaderResources(0, 1, &Fill->Tex->TexSv);
		DIRECTX.Context->DrawIndexed(36, 0, 0);

	}



};
