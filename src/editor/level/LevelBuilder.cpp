////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/editor/level/LevelBuilder.h>
////////////////////////////////////////////////////////////
namespace nero
{
    LevelBuilder::LevelBuilder(GameLevel::Context context)
        : m_LevelContext(context)
        , m_RenderContext(nullptr)
        , m_LevelOpened(false)
        , m_SelectedChunk(nullptr)
        , m_ChunkTable()
        , m_ChunkCount(0)

    {
    }

    LevelBuilder::~LevelBuilder()
    {
    }

    void LevelBuilder::loadResource()
    {
        m_LevelContext.resourceManager->loadDirectory(
            file::getPath({m_LevelContext.levelDirectory, "resource"}));
    }

    ResourceManager::Ptr LevelBuilder::getResourceManager()
    {
        return m_LevelContext.resourceManager;
    }

    std::string LevelBuilder::getLevelName()
    {
        return m_LevelContext.levelSetting->getString("level_name");
    }

    std::string LevelBuilder::getResourceFoler()
    {
        return file::getPath({m_LevelContext.levelDirectory, "resource"});
    }

    Setting::Ptr LevelBuilder::getLevelSetting()
    {
        return m_LevelContext.levelSetting;
    }

    ChunkBuilder::Ptr LevelBuilder::addChunk()
    {
        // create
        ChunkBuilder::Ptr chunkBuilder = std::make_shared<ChunkBuilder>();
        chunkBuilder->setChunkId(++m_ChunkCount);
        chunkBuilder->setChunkName(std::string("world chunk ") +
                                   toString(chunkBuilder->getChunkId()));
        chunkBuilder->setSelected(false);
        chunkBuilder->setVisible(true);
        chunkBuilder->setLoadWithLevel(false);

        WorldBuilder::Ptr worldBuilder = chunkBuilder->getWorldBuilder();
        worldBuilder->setRenderContext(m_RenderContext);
        worldBuilder->setRenderTexture(m_LevelContext.renderTexture);
        worldBuilder->setResourceManager(m_LevelContext.resourceManager);
        worldBuilder->init();

        // register
        m_ChunkTable.push_back(chunkBuilder);

        // select
        setSelectedChunk(chunkBuilder);

        return chunkBuilder;
    }

    void LevelBuilder::removeChunk()
    {
        // We should always have one chunk
        if(m_ChunkTable.size() == 1)
            return;

        for(auto it = m_ChunkTable.begin(); it != m_ChunkTable.end(); it++)
        {
            if(m_SelectedChunk->getChunkId() == (*it)->getChunkId())
            {
                // Select the next
                if(it == m_ChunkTable.begin())
                    m_SelectedChunk = *(it + 1);
                else
                    m_SelectedChunk = *(it - 1);

                // delete file from disk
                auto chunkFile =
                    file::getPath({m_LevelContext.levelDirectory, "chunk", (*it)->getChunkName()},
                                  StringPool.EXT_NERO);

                file::removeFile(chunkFile);

                // remove level builder from memory
                m_ChunkTable.erase(it);

                break;
            }
        }
    }

    std::vector<ChunkBuilder::Ptr>& LevelBuilder::getChunkTable()
    {
        return m_ChunkTable;
    }

    ChunkBuilder::Ptr LevelBuilder::getSelectedChunk()
    {
        return m_SelectedChunk;
    }

    void LevelBuilder::setSelectedChunk(ChunkBuilder::Ptr worldChunk)
    {
        m_SelectedChunk = worldChunk;
    }

    void LevelBuilder::setRenderContext(const RenderContext::Ptr& renderContext)
    {
        m_RenderContext = renderContext;
    }

    void LevelBuilder::saveGameLevel()
    {
        // Delete all chunk first (necessary when chunks are renamed)
        file::removeDirectory(file::getPath({m_LevelContext.levelDirectory, "chunk"}));
        file::createDirectory(file::getPath({m_LevelContext.levelDirectory, "chunk"}));
        // chunk
        for(auto chunk : m_ChunkTable)
        {
            // update is_seleted
            chunk->getChunkId() == m_SelectedChunk->getChunkId() ? chunk->setSelected(true)
                                                                 : chunk->setSelected(false);

            // save chunk
            file::saveFile(
                file::getPath({m_LevelContext.levelDirectory, "chunk", chunk->getChunkName()},
                              StringPool.EXT_NERO),
                chunk->saveChunk().dump(3),
                true);
        }

        // setting
        m_LevelContext.levelSetting->setInt("chunk_count", m_ChunkCount);
        m_LevelContext.levelSetting->setBool("opened", m_LevelOpened);

        file::saveFile(
            file::getPath({m_LevelContext.levelDirectory, "setting"}, StringPool.EXT_NERO),
            m_LevelContext.levelSetting->toString(),
            true);
    }

    void LevelBuilder::loadGameLevel()
    {
        std::experimental::filesystem::path chunkDirectory(
            file::getPath({m_LevelContext.levelDirectory, "chunk"}));

        std::experimental::filesystem::directory_iterator it{chunkDirectory};
        while(it != std::experimental::filesystem::directory_iterator{})
        {
            loadChunk(it->path().string());

            it++;
        }

        m_LevelOpened = m_LevelContext.levelSetting->getBool("opened");
        m_ChunkCount  = m_LevelContext.levelSetting->getInt("chunk_count");
    }

    void LevelBuilder::loadChunk(const std::string& fileName)
    {
        ChunkBuilder::Ptr chunkBuilder = std::make_shared<ChunkBuilder>();
        WorldBuilder::Ptr worldBuilder = chunkBuilder->getWorldBuilder();

        worldBuilder->setRenderContext(m_RenderContext);
        worldBuilder->setRenderTexture(m_LevelContext.renderTexture);
        worldBuilder->setResourceManager(m_LevelContext.resourceManager);

        chunkBuilder->loadChunk(file::loadJson(fileName, true));

        if(chunkBuilder->isSelected())
        {
            setSelectedChunk(chunkBuilder);
        }

        m_ChunkTable.push_back(chunkBuilder);
    }

    void LevelBuilder::setOpened(const bool& opened)
    {
        m_LevelOpened = opened;
    }

    bool LevelBuilder::isOpened() const
    {
        return m_LevelOpened;
    }

    std::string LevelBuilder::getResourceDirectory() const
    {
        return file::getPath({m_LevelContext.levelDirectory, "resource"});
    }

    void LevelBuilder::render() const
    {
        for(const auto& worldChunk : m_ChunkTable)
        {
            if(worldChunk->isVisible())
            {
                worldChunk->getWorldBuilder()->render();
            }
        }
    }

    GameLevel::Context LevelBuilder::getLevelContext() const
    {
        return m_LevelContext;
    }

    void LevelBuilder::buildLevel(Object::Ptr                        levelRoot,
                                  std::shared_ptr<ltbl::LightSystem> lightManager)
    {
        for(const auto& worldChunk : m_ChunkTable)
        {
            if(worldChunk->isVisible())
            {
                levelRoot->addChild(worldChunk->getWorldBuilder()->buildScene(lightManager));
            }
        }
    }

} // namespace nero
