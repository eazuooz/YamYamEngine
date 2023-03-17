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
		, mCount(144)
		, mStartSize(Vector4::Zero)
		, mEndSize(Vector4::Zero)
		, mStartColor(Vector4::One)
		, mEndColor(Vector4::One)
		, mMinLifeTime(0.0f)
		, mMaxLifeTime(0.0f)
	
	{
		std::shared_ptr<Mesh> mesh = Resources::Find<Mesh>(L"SpriteDefaultMesh");
		SetMesh(mesh);

		std::shared_ptr<Material> material= Resources::Find<Material>(L"ParticleMaterial");
		SetMaterial(material);

		Particle particles[1000] = {};
		Vector4 startPos(-800.0f, -450.0f, 0.0f, 0.0f);
		for (int i = 0; i < 9; ++i)
		{
			for (int j = 0; j < 16; ++j)
			{
				particles[16 * i + j].position = startPos + Vector4(j * 100, i * 100, 0.f, 0.f);
			}
		}

		mBuffer = new StructedBuffer();
		mBuffer->Create(sizeof(Particle), mCount, eSRVType::None, particles);
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
	}

	void ParticleSystem::Render()
	{
		GetOwner()->GetComponent<Transform>()->BindConstantBuffer();
		mBuffer->SetPipline(eShaderStage::VS, 15);
		mBuffer->SetPipline(eShaderStage::PS, 15);

		GetMaterial()->Bind();

		GetMesh()->RenderInstanced(mCount);
	}

}
