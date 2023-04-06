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
		, mCount(100)
		, mStartSize(Vector4(50.0f, 50.0f, 1.0f, 0.0f))
		, mEndSize(Vector4(10.0f, 10.0f, 1.0f, 0.0f))
		, mStartColor(Vector4::One)
		, mEndColor(Vector4::One)
		, mMinLifeTime(0.2f)
		, mMaxLifeTime(0.1f)
		, mFrequency(3.0f)
		, mCBData{}
		, mSpawnRange(500.0f)
		, mWorldSpawn(1)
		, mMinSpeed(100.0f)
		, mMaxSpeed(300.0f)
	
	{
		std::shared_ptr<Mesh> mesh = Resources::Find<Mesh>(L"PointMesh");
		SetMesh(mesh);

		std::shared_ptr<Material> material = Resources::Find<Material>(L"ParticleMaterial");
		SetMaterial(material);

		std::shared_ptr<Texture> tex = Resources::Find<Texture>(L"BubbleParticle");
		material->SetTexture(eTextureSlot::T0, tex);

		mCS = Resources::Find<ParticleShader>(L"ParticleShaderCS");

		Particle particles[200] = {};
		Vector4 startPos(0.0f, 0.0f, 0.0f, 0.0f);
		for (size_t i = 0; i < mCount; i++)
		{
			particles[i].active = 0;
			particles[i].position = Vector4(0.0f, 0.0f, 20.0f, 1.0f);
			particles[i].direction = 
				Vector4(cosf((float)i * (XM_2PI / (float)mCount))
					, sinf((float)i * (XM_2PI / 100.f))
					, 0.0f, 1.0f);
			particles[i].speed = 0.0f;
		}

		mBuffer = new StructedBuffer();
		mBuffer->Create(sizeof(Particle), mCount, eViewType::UAV, particles);

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

	void ParticleSystem::FixedUpdate()
	{
		
		//pos.x += 500.0f * Time::DeltaTime();
		//GetOwner()->GetComponent<Transform>()->SetPosition(pos);

		// 파티클 생성 시간
		float fAliveTime = 1.f / mFrequency;

		// 누적시간
		mTime += Time::DeltaTime();

		// 누적시간이 파티클 생성 시간을 넘어서면


		if (fAliveTime < mTime)
		{
			float f = (mTime / fAliveTime);
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

		
		mCBData.elementCount = mBuffer->GetStride();
		mCBData.deltaTime = Time::DeltaTime();
		mCBData.elapsedTime += Time::DeltaTime();
		mCBData.isWorld = mWorldSpawn;
		Vector3 pos = GetOwner()->GetComponent<Transform>()->GetPosition();
		mCBData.ObjectWorldPos = Vector4(pos.x, pos.y, pos.z, 1.0f);
		mCBData.startSize = mStartSize;
		mCBData.endSize = mEndSize;
		mCBData.spawnRange = mSpawnRange;
		mCBData.minLifeTime = mMinLifeTime;
		mCBData.maxLifeTime = mMaxLifeTime;
		mCBData.minSpeed = mMinSpeed;
		mCBData.maxSpeed = mMaxSpeed;



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

		GetMesh()->RenderInstanced(mCount);

		//clear
		mBuffer->Clear();
	}

}
