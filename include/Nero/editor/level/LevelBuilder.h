////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
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

        void                 loadResource();
        Setting::Ptr         getLevelSetting();
        std::string          getLevelName();
        std::string          getResourceFoler();
        ResourceManager::Ptr getResourceManager();
        void                 setEditorSetting(const Setting::Ptr& setting);
        void                 saveGameLevel();
        void                 loadGameLevel();
        void                 setRenderContext(const RenderContext::Ptr& renderContext);
        void              setRenderTexture(const std::shared_ptr<sf::RenderTexture>& renderTexture);
        // chunk
        ChunkBuilder::Ptr addChunk();
        void              removeChunk();
        std::vector<ChunkBuilder::Ptr>& getChunkTable();
        ChunkBuilder::Ptr               getSelectedChunk();
        void                            setSelectedChunk(ChunkBuilder::Ptr worldChunk);
        void                            loadChunk(const std::string& fileName);
        //
        void                            setOpened(const bool& opened);
        bool                            isOpened() const;
        std::string                     getResourceDirectory() const;
        void                            render() const;

      private:
        // Game Level
        GameLevel::Ptr                     m_GameLevel;
        Setting::Ptr                       m_LevelSetting;
        // render
        RenderContext::Ptr                 m_RenderContext;
        std::shared_ptr<sf::RenderTexture> m_RenderTexture;
        Setting::Ptr                       m_EditorSetting;
        // chunk
        ChunkBuilder::Ptr                  m_SelectedChunk;
        std::vector<ChunkBuilder::Ptr>     m_ChunkTable;
        int                                m_ChunkCount;
        //
        bool                               m_Opened;
    };
} // namespace nero
#endif // LEVELBUILDER_H
