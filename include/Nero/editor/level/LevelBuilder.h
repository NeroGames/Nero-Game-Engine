////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef LEVELBUILDER_H
#define LEVELBUILDER_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/scene/GameLevel.h>
#include <Nero/editor/level/ChunkBuilder.h>
// Std
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    class LevelBuilder
    {
      public: // utility
        typedef std::shared_ptr<LevelBuilder> Ptr;

      public:
        LevelBuilder(GameLevel::Context context);
        ~LevelBuilder();

        void                            loadResource();
        Setting::Ptr                    getLevelSetting();
        std::string                     getLevelName();
        std::string                     getResourceFoler();
        ResourceManager::Ptr            getResourceManager();
        void                            saveGameLevel();
        void                            loadGameLevel();
        void                            setRenderContext(const RenderContext::Ptr& renderContext);
        // chunk
        ChunkBuilder::Ptr               addChunk();
        void                            removeChunk();
        std::vector<ChunkBuilder::Ptr>& getChunkTable();
        ChunkBuilder::Ptr               getSelectedChunk();
        void                            setSelectedChunk(ChunkBuilder::Ptr worldChunk);
        void                            loadChunk(const std::string& fileName);
        //
        void                            setOpened(const bool& opened);
        bool                            isOpened() const;
        std::string                     getResourceDirectory() const;
        void                            render() const;
        void                            buildLevel(Object::Ptr levelRoot);
        GameLevel::Context              getLevelContext() const;

      private:
        // Level Context
        GameLevel::Context             m_LevelContext;
        RenderContext::Ptr             m_RenderContext;
        bool                           m_LevelOpened;
        // Chunk
        ChunkBuilder::Ptr              m_SelectedChunk;
        std::vector<ChunkBuilder::Ptr> m_ChunkTable;
        int                            m_ChunkCount;
    };
} // namespace nero
#endif // LEVELBUILDER_H
