#include "ParticleSystem.h"
#include "yaMesh.h"
#include "yaResources.h"
#include "yaMaterial.h"
#include "yaStructedBuffer.h"
#include "yaTransform.h"
#include "yaGameObject.h"
#include "yaTime.h"
#include "yaConstantBuffer.h"
#include "yaRenderer.h"

namespace ya
{
	ParticleSystem::ParticleSystem()
		: BaseRenderer(eComponentType::ParticleSystem)
		, mMaxParticles(100)
		, mStartSize(Vector4(1.0f, 1.0f, 1.0f, 1.0f))
		, mStartColor(Vector4(1.0f, 0.0f, 1.0f, 1.0f))
		, mStartLifeTime(2.0f)
		, mFrequency(3.0f)
		, mCBData{}
		, mRadius(500.0f)
		, mSimulationSpace(eSimulationSpace::World)
		, mStartSpeed(100.0f)
		, mTime(0.0f)
		
	{
		std::shared_ptr<Mesh> mesh = Resources::Find<Mesh>(L"PointMesh");
		SetMesh(mesh);

		std::shared_ptr<Material> material = Resources::Find<Material>(L"ParticleMaterial");
		SetMaterial(material);

		std::shared_ptr<Texture> tex = Resources::Find<Texture>(L"BubbleParticle");
		material->SetTexture(eTextureType::Albedo, tex);

		mCS = Resources::Find<ParticleShader>(L"ParticleShaderCS");

		Particle particles[200] = {};
		for (size_t i = 0; i < mMaxParticles; i++)
		{
			particles[i].active = 0;
			particles[i].position = Vector4(0.0f, 0.0f, 20.0f, 1.0f);
			particles[i].direction = 
				Vector4(cosf((float)i * (XM_2PI / (float)mMaxParticles))
					, sinf((float)i * (XM_2PI / 100.f))
					, 0.0f, 1.0f);
			particles[i].speed = 0.0f;
		}

		mBuffer = new StructedBuffer();
		mBuffer->Create(sizeof(Particle), mMaxParticles, eViewType::UAV, particles);

		mSharedBuffer = new StructedBuffer();
		mSharedBuffer->Create(sizeof(ParticleShared), 1, eViewType::UAV, nullptr, true);
	}

	ParticleSystem::~ParticleSystem()
	{
		if (nullptr != mBuffer)
			delete mBuffer;

		if (nullptr != mSharedBuffer)
			delete mSharedBuffer;
	}

	void ParticleSystem::Initialize()
	{
	}

	void ParticleSystem::Update()
	{
	}

	void ParticleSystem::LateUpdate()
	{
		float aliveTime = 1.f / mFrequency;
		mTime += Time::DeltaTime();

		if (aliveTime < mTime)
		{
			float f = (mTime / aliveTime);
			UINT iAliveCount = (UINT)f;
			mTime = f - floor(f);
			
			ParticleShared shared = {2,};
			mSharedBuffer->SetData(&shared, 1);
		}
		else
		{
			ParticleShared shared = { };
			mSharedBuffer->SetData(&shared, 1);
		}

		Vector3 pos = GetOwner()->GetComponent<Transform>()->GetPosition();
		mMaxParticles = mBuffer->GetStride();
		mCBData.maxParticles = mMaxParticles;
		mCBData.deltaTime = Time::DeltaTime();
		mCBData.elapsedTime += Time::DeltaTime();
		mCBData.simulationSpace = (UINT)mSimulationSpace;
		mCBData.position = Vector4(pos.x, pos.y, pos.z, 1.0f);
		mCBData.startSize = mStartSize;
		mCBData.radius = mRadius;
		mCBData.startLifeTime = mStartLifeTime;
		mCBData.startSpeed = mStartSpeed;
		mCBData.startColor = mStartColor;

		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::ParticleSystem];
		cb->SetData(&mCBData);
		cb->Bind(eShaderStage::ALL);

		mCS->SetStructedBuffer(mBuffer);
		mCS->SetSharedStructedBuffer(mSharedBuffer);
		mCS->OnExcute();
	}

	void ParticleSystem::Render()
	{
		GetOwner()->GetComponent<Transform>()->BindConstantBuffer();
		mBuffer->BindSRV(eShaderStage::GS, 16);

		GetMaterial()->Bind();

		GetMesh()->RenderInstanced(mMaxParticles);

		//clear
		mBuffer->Clear();
	}

}
