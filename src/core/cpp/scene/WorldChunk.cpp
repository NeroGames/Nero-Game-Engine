////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/core/cpp/scene/WorldChunk.h>
#include <Nero/core/cpp/utility/String.h>
/////////////////////////////////////////////////////////////
namespace nero
{
	WorldChunk::WorldChunk():
		 m_ChunkId(-1)
		,m_ChunkName(StringPool.BLANK)
		,m_Visible(false)
		,m_Selected(false)
		,m_LoadWithLevel(false)
	{

	}

	WorldChunk::~WorldChunk()
	{

	}

	int WorldChunk::getChunkId() const
	{
		return m_ChunkId;
	}

	std::string WorldChunk::getName() const
	{
		return m_ChunkName;
	}

	bool WorldChunk::isVisible() const
	{
		return m_Visible;
	}

	bool WorldChunk::isSelected() const
	{
		return m_Selected;
	}

	bool WorldChunk::isLoadWithLevel() const
	{
		return m_LoadWithLevel;
	}

	void WorldChunk::setChunkId(const int& chunkId)
	{
		m_ChunkId = chunkId;
	}

	void WorldChunk::setName(const std::string& chunkName)
	{
		m_ChunkName = chunkName;
	}

	void WorldChunk::setVisible(const bool& visible)
	{
		m_Visible = visible;
	}

	void WorldChunk::setSelected(const bool& selected)
	{
		m_Selected = selected;
	}

	void WorldChunk::setLoadWithLevel(const bool &loadWithLevel)
	{
		m_LoadWithLevel = loadWithLevel;
	}
}
