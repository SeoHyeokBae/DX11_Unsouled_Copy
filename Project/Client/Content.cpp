#include "pch.h"
#include "Content.h"

#include "TreeUI.h"

Content::Content()
	: UI("Content","##Content")
{
	m_Tree = new TreeUI("ContentTree");
	AddChildUI(m_Tree);
}

Content::~Content()
{
}

void Content::render_update()
{
}
