#include "ParticleSystem.h"
#include "yaMesh.h"
#include "yaResources.h"
#include "yaMaterial.h"
#include "yaStructedBuffer.h"
#include "yaTransform.h"
#include "yaGameObject.h"


namespace ya
{
	ParticleSystem::ParticleSystem()
		: BaseRenderer(eComponentType::ParticleSystem)
		, mCount(100)
		, mStartSize(Vector4::Zero)
		, mEndSize(Vector4::Zero)
		, mStartColor(Vector4::One)
		, mEndColor(Vector4::One)
		, mMinLifeTime(0.0f)
		, mMaxLifeTime(0.0f)
	
	{
		std::shared_ptr<Mesh> mesh = Resources::Find<Mesh>(L"PointMesh");
		SetMesh(mesh);

		std::shared_ptr<Material> material = Resources::Find<Material>(L"ParticleMaterial");
		SetMaterial(material);

		std::shared_ptr<Texture> tex = Resources::Find<Texture>(L"SmokeParticle");
		material->SetTexture(eTextureSlot::T0, tex);

		mCS = Resources::Find<ParticleShader>(L"ParticleShaderCS");

		Particle particles[1000] = {};
		Vector4 startPos(0.0f, 0.0f, 0.0f, 0.0f);
		for (size_t i = 0; i < mCount; i++)
		{
			particles[i].active = 1;
			particles[i].position = Vector4(0.0f, 0.0f, 20.0f, 1.0f);
			particles[i].direction = 
				Vector4(cosf((float)i * (XM_2PI / (float)mCount))
					, sinf((float)i * (XM_2PI / 100.f))
					, 0.0f, 1.0f);
			particles[i].speed = 200.0f;
		}

		mBuffer = new StructedBuffer();
		mBuffer->Create(sizeof(Particle), mCount, eViewType::UAV, particles);

		//mCS = std::make_shared<ParticleShader>();
	}

	ParticleSystem::~ParticleSystem()
	{
		if (nullptr != mBuffer)
			delete mBuffer;
	}

	void ParticleSystem::Initialize()
	{
	}

	void ParticleSystem::Update()
	{
	}

	void ParticleSystem::FixedUpdate()
	{
		mCS->SetParticleBuffer(mBuffer);
		mCS->OnExcute();
	}

	void ParticleSystem::Render()
	{
		GetOwner()->GetComponent<Transform>()->BindConstantBuffer();
		mBuffer->BindSRV(eShaderStage::VS, 15);
		mBuffer->BindSRV(eShaderStage::GS, 15);
		mBuffer->BindSRV(eShaderStage::PS, 15);

		GetMaterial()->Bind();

		GetMesh()->RenderInstanced(mCount);

		//clear
		mBuffer->Clear();
	}

}
