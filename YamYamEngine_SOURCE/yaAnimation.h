#pragma once
#include "yaEntity.h"
#include "yaTexture.h"
#include "yaMeshData.h"
#include "yaFbxLoader.h"

namespace ya
{
	class Animation : public Entity
	{
	public:
		struct Sprite
		{
			Vector2 leftTop;	// ���� ��� �ȼ���ǥ
			Vector2 size;		// ���� ������� ���� �߶� ���� ���� �ȼ� ����
			Vector2 offset;
			Vector2 atlasSize;
			float duration;			// �ش� ������ �����ð�
			//Event evetn;

			Sprite()
				: leftTop()
				, size()
				, offset()
				, atlasSize()
				, duration(0.0f)
			{


			}
		};
		struct AnimationClip
		{
			std::wstring name;
			int startFrame;
			int endFrame;
			int frameLength;

			double startTime;
			double endTime;
			double timeLength;
			UINT mode;
		};
		
		Animation();
		~Animation();

		void Update();
		void FixedUpdate();
		void Render();


		void Create(const std::wstring& name, std::shared_ptr<graphics::Texture> atlas
			, Vector2 leftTop, Vector2 size, Vector2 offset
			, float columnLegth, UINT spriteLength, float duration);
		void Create(const std::wstring& name, FbxLoader::AnimationClip* clip);
		void Bind(int boneCount);
		void BindSprite();
		void BindClip(int boneCount);
		void Reset();
		void Clear();
		void UpdateSprite();
		void UpdateBone();
		void UpdateFrameIndex();
		
		bool IsComplete() const;
		void SetType(eAnimationType type) { mType = type; }
		eAnimationType GetType() { return mType; }
		void SetAnimator(class Animator* animator) { mAnimator = animator; }
		

	private:
		eAnimationType mType;
		class Animator* mAnimator;
		
		bool mbComplete;
		float mTime;

		//2D
		std::shared_ptr<graphics::Texture> mAtlas;
		std::vector<Sprite> mSpriteSheet;
		int mIndex;
		
		////3D
		AnimationClip mClip;
		int mFrameIndex;
		int mNextFrameIndex;
		float mFrameRatio;
	};
}
