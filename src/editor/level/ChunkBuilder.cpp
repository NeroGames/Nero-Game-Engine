////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/editor/level/ChunkBuilder.h>
#include <Nero/core/cpp/utility/String.h>
/////////////////////////////////////////////////////////////
namespace nero
{
	WorldChunkBuilder::WorldChunkBuilder():
		 m_ChunkId(-1)
		,m_ChunkName(StringPool.BLANK)
		,m_Visible(false)
		,m_Selected(false)
		,m_LoadWithLevel(false)
		,m_WorldBuilder(nullptr)
	{

	}

	WorldChunkBuilder::~WorldChunkBuilder()
	{

	}

	int WorldChunkBuilder::getChunkId() const
	{
		return m_ChunkId;
	}

	std::string WorldChunkBuilder::getName() const
	{
		return m_ChunkName;
	}

	bool WorldChunkBuilder::isVisible() const
	{
		return m_Visible;
	}

	bool WorldChunkBuilder::isSelected() const
	{
		return m_Selected;
	}

	bool WorldChunkBuilder::isLoadWithLevel() const
	{
		return m_LoadWithLevel;
	}

	void WorldChunkBuilder::setChunkId(const int& chunkId)
	{
		m_ChunkId = chunkId;
	}

	void WorldChunkBuilder::setName(const std::string& chunkName)
	{
		m_ChunkName = chunkName;
	}

	void WorldChunkBuilder::setVisible(const bool& visible)
	{
		m_Visible = visible;
	}

	void WorldChunkBuilder::setSelected(const bool& selected)
	{
		m_Selected = selected;
	}

	void WorldChunkBuilder::setLoadWithLevel(const bool &loadWithLevel)
	{
		m_LoadWithLevel = loadWithLevel;
	}

	WorldBuilder::Ptr WorldChunkBuilder::getWorldBuilder()
	{
		return m_WorldBuilder;
	}

	void WorldChunkBuilder::setWorldBuilder(WorldBuilder::Ptr worldBuilder)
	{
		m_WorldBuilder = worldBuilder;
	}
}
