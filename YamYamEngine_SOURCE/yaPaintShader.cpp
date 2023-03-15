#include "yaPaintShader.h"



namespace ya::graphics
{
	void PaintShader::Binds()
	{
		//target 텍스처를 u0번에 바인딩
		mTarget->BindUnorderedAccessViews(0);

		mGroupX = mTarget->GetWidth() / mThreadGroupCountX + 1;
		mGroupY = mTarget->GetHeight() / mThreadGroupCountY + 1;;
		mGroupZ = 1;
	}
	void PaintShader::Clear()
	{
		mTarget->ClearUnorderedAccessViews(0);
	}
}

