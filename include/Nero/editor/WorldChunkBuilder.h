////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef WORLDCHUNKBUILDER_H
#define WORLDCHUNKBUILDER_H
///////////////////////////HEADERS///////////////////////////
//Nero
#include <Nero/editor/WorldBuilder.h>
//STD
#include <memory>
/////////////////////////////////////////////////////////////
namespace nero
{
	class WorldChunkBuilder
	{
		public: //utility
			typedef std::shared_ptr	<WorldChunkBuilder> Ptr;

		public:
								WorldChunkBuilder();
							   ~WorldChunkBuilder();

			//getters
			int					getChunkId()		const;
			std::string			getName()			const;
			bool				isVisible()			const;
			bool				isSelected()		const;
			bool				isLoadWithLevel()	const;
			//setters
			void				setChunkId(const int& chunkId);
			void				setName(const std::string& chunkName);
			void				setVisible(const bool& visible);
			void				setSelected(const bool& selected);
			void				setLoadWithLevel(const bool& loadWithLevel);
			WorldBuilder::Ptr	getWorldBuilder();
			void				setWorldBuilder(WorldBuilder::Ptr worldBuilder);

		private:
			int					m_ChunkId;
			std::string			m_ChunkName;
			bool				m_Visible;
			bool				m_Selected;
			bool				m_LoadWithLevel;
			WorldBuilder::Ptr	m_WorldBuilder;
	};
}

#endif // WORLDCHUNKBUILDER_H
