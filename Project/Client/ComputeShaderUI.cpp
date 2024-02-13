#include "pch.h"
#include "ComputeShaderUI.h"


ComputeShaderUI::ComputeShaderUI()
	: AssetUI("ComputeShader", "##ComputeShader", ASSET_TYPE::COMPUTE_SHADER)
{
}

ComputeShaderUI::~ComputeShaderUI()
{
}

void ComputeShaderUI::render_update()
{
	AssetUI::render_update();
}
