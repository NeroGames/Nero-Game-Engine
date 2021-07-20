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
	ChunkBuilder::ChunkBuilder():
		 m_ChunkId(-1)
		,m_ChunkName(StringPool.BLANK)
		,m_Visible(false)
		,m_Selected(false)
		,m_AutoLoad(false)
		,m_WorldBuilder(std::make_shared<WorldBuilder>())
	{

	}

	ChunkBuilder::~ChunkBuilder()
	{

	}

	int ChunkBuilder::getChunkId() const
	{
		return m_ChunkId;
	}

	std::string ChunkBuilder::getChunkName() const
	{
		return m_ChunkName;
	}

	bool ChunkBuilder::isVisible() const
	{
		return m_Visible;
	}

	bool ChunkBuilder::isSelected() const
	{
		return m_Selected;
	}

	bool ChunkBuilder::isAutoLoad() const
	{
		return m_AutoLoad;
	}

	void ChunkBuilder::setChunkId(const int& chunkId)
	{
		m_ChunkId = chunkId;
	}

	void ChunkBuilder::setChunkName(const std::string& chunkName)
	{
		m_ChunkName = chunkName;
	}

	void ChunkBuilder::setVisible(const bool& visible)
	{
		m_Visible = visible;
	}

	void ChunkBuilder::setSelected(const bool& selected)
	{
		m_Selected = selected;
	}

	void ChunkBuilder::setAutoLoad(const bool& autoLoad)
	{
		m_AutoLoad = autoLoad;
	}

	WorldBuilder::Ptr ChunkBuilder::getWorldBuilder() const
	{
		return m_WorldBuilder;
	}

	nlohmann::json ChunkBuilder::saveChunk() const
	{
		nlohmann::json chunkSaved = nlohmann::json::object();

		chunkSaved["chunk_id"]		= m_ChunkId;
		chunkSaved["chunk_name"]	= m_ChunkName;
		chunkSaved["visible"]		= m_Visible;
		chunkSaved["selected"]		= m_Selected;
		chunkSaved["auto_load"]		= m_AutoLoad;
		chunkSaved["game_world"]	= m_WorldBuilder->saveScene();

		return chunkSaved;
	}

	void ChunkBuilder::loadChunk(const  nlohmann::json& chunkSaved)
	{
		m_ChunkId	= chunkSaved["chunk_id"];
		m_ChunkName	= chunkSaved["chunk_name"];
		m_Visible	= chunkSaved["visible"];
		m_Selected	= chunkSaved["selected"];
		m_AutoLoad	= chunkSaved["auto_load"];

		m_WorldBuilder->loadScene(chunkSaved["game_world"]);
	}
}
