#include "yaPaintShader.h"



namespace ya::graphics
{
	PaintShader::PaintShader()
		: ComputeShader(16, 32, 1)
	{
	}
	PaintShader::~PaintShader()
	{
	}
	void PaintShader::Binds()
	{
		//target �ؽ�ó�� u0���� ���ε�
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

