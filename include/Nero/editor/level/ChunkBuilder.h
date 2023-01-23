////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef CHUNKBUILDER_H
#define CHUNKBUILDER_H
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/editor/level/WorldBuilder.h>
// Std
#include <memory>
/////////////////////////////////////////////////////////////
namespace nero
{
    class ChunkBuilder
    {
      public: // utility
        typedef std::shared_ptr<ChunkBuilder> Ptr;

      public:
        ChunkBuilder();
        ~ChunkBuilder();
        // set
        void              setChunkId(const int& chunkId);
        void              setChunkName(const std::string& chunkName);
        void              setVisible(const bool& visible);
        void              setSelected(const bool& selected);
        void              setAutoLoad(const bool& autoLoad);
        // get
        int               getChunkId() const;
        std::string       getChunkName() const;
        bool              isVisible() const;
        bool              isSelected() const;
        bool              isAutoLoad() const;
        WorldBuilder::Ptr getWorldBuilder() const;
        //
        nlohmann::json    saveChunk() const;
        void              loadChunk(const nlohmann::json& chunkSaved);

      private:
        // primary
        int               m_ChunkId;
        std::string       m_ChunkName;
        bool              m_Visible;
        bool              m_Selected;
        bool              m_AutoLoad;
        // game world
        WorldBuilder::Ptr m_WorldBuilder;
    };
} // namespace nero
#endif // CHUNKBUILDER_H
