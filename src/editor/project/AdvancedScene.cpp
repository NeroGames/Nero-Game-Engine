////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/editor/project/AdvancedScene.h>
#include <Nero/core/cpp/utility/Utility.h>
////////////////////////////////////////////////////////////
namespace nero
{
    AdvancedScene::AdvancedScene()
        : m_LevelBuilder(nullptr)
        , m_RegisteredLevelTable()
        , m_SelectedScreen(nullptr)
        , m_EditorSetting(nullptr)
        , m_ProjectSetting(nullptr)
        , m_RenderContext(nullptr)
        , m_RenderTexture(nullptr)
    {
    }

    void AdvancedScene::init()
    {
        Setting sceneSetting;
        sceneSetting.loadSetting(
            file::getPath({m_ProjectSetting->getString("project_directory"), "Scene", "scene"},
                          StringPool.EXT_NERO),
            true,
            true);
        m_RegisteredLevelTable      = sceneSetting.getStringTable("level_table");

        // TODO
        std::string resourceSetting = file::getPath({"setting", "resource"}, StringPool.EXT_JSON);
        m_EditorSetting             = std::make_shared<Setting>();
        m_EditorSetting->loadSetting(resourceSetting, true);
    }

    void AdvancedScene::clear()
    {
        m_LevelBuilder = nullptr;
        m_RegisteredLevelTable.clear();
    }

    std::string AdvancedScene::createLevel(const Parameter& parameter)
    {
        // Generate parameters
        std::string levelName = string::trim(parameter.getString("level_name"));
        std::string levelID   = string::formatString(levelName, string::Format::COMPACT_LOWER);
        std::string className =
            string::formatString(levelName, string::Format::CAMEL_CASE_UPPER) + "GameLevel";
        std::string headerGard =
            string::formatString(levelName, string::Format::COMPACT_UPPER) + "_H";

        // generate source file
        // paremeter
        std::string headerFile = file::loadText("template/cpp_project/CppGameLevel.h");
        std::string sourceFile = file::loadText("template/cpp_project/CppGameLevel.cpp");
        // file 1 : header
        boost::algorithm::replace_all(headerFile, "::GameLevelClass::", className);
        boost::algorithm::replace_all(headerFile, "::HeaderGard::", headerGard);
        boost::algorithm::replace_all(headerFile,
                                      "::Namespace::",
                                      m_ProjectSetting->getString("project_namespace"));
        boost::algorithm::replace_all(headerFile,
                                      "::ProjectName::",
                                      m_ProjectSetting->getString("project_name"));
        boost::algorithm::replace_all(headerFile,
                                      "::ProjectLead::",
                                      m_ProjectSetting->getString("project_lead"));
        boost::algorithm::replace_all(headerFile,
                                      "::CoyprightDate::",
                                      toString(datetime::getCurrentDateTime().date().year()));
        // file 2 : source
        boost::algorithm::replace_all(sourceFile, "::GameLevelClass::", className);
        boost::algorithm::replace_all(sourceFile,
                                      "::Namespace::",
                                      m_ProjectSetting->getString("project_namespace"));
        boost::algorithm::replace_all(sourceFile,
                                      "::ProjectName::",
                                      m_ProjectSetting->getString("project_name"));
        boost::algorithm::replace_all(sourceFile,
                                      "::ProjectLead::",
                                      m_ProjectSetting->getString("project_lead"));
        boost::algorithm::replace_all(sourceFile,
                                      "::CoyprightDate::",
                                      toString(datetime::getCurrentDateTime().date().year()));
        // save file
        file::saveFile(
            file::getPath(
                {m_ProjectSetting->getString("source_directory"), "cpp", "level", className},
                StringPool.EXT_H),
            headerFile);
        file::saveFile(
            file::getPath(
                {m_ProjectSetting->getString("source_directory"), "cpp", "level", className},
                StringPool.EXT_CPP),
            sourceFile);

        // generate level directory
        // directory
        std::string levelDirectory =
            file::getPath({m_ProjectSetting->getString("project_directory"),
                           "Scene",
                           "level",
                           boost::algorithm::to_lower_copy(levelName)});
        file::createDirectory(levelDirectory);
        file::createDirectory(file::getPath({levelDirectory, "chunk"}));
        file::createDirectory(file::getPath({levelDirectory, "resource"}));
        ResourceManager::buildDirectory(file::getPath({levelDirectory, "resource"}));
        // level document
        Parameter document;
        document.setString("creation_date",
                           datetime::formatDateTime(datetime::getCurrentDateTime()));
        document.setString("level_name", levelName);
        document.setString("level_id", levelID);
        document.setString("template", parameter.getString("template"));
        file::saveFile(file::getPath({levelDirectory, "level"}, StringPool.EXT_NERO),
                       document.toString());
        // setting
        Setting setting;
        setting.setString("level_name", levelName);
        setting.setString("level_id", levelID);
        setting.setBool("enable_physics", parameter.getBool("enable_physics"));
        setting.setBool("enable_light", parameter.getBool("enable_light"));
        setting.setInt("chunk_count", 0);
        setting.setBool("opened", false);
        setting.setSetting("resource", m_EditorSetting->getSetting("resource"));
        file::saveFile(file::getPath({levelDirectory, "setting"}, StringPool.EXT_NERO),
                       setting.toString());

        registerLevel(levelName);

        return levelName;
    }

    void AdvancedScene::openLevel(const std::string& levelName)
    {
        if(levelName == StringPool.BLANK)
            return;

        if(m_LevelBuilder)
            closeSelectedLevel();

        // Build level directory
        std::string levelDirectory =
            file::getPath({m_ProjectSetting->getString("project_directory"),
                           "Scene",
                           "level",
                           boost::algorithm::to_lower_copy(levelName)});

        // Load level setting
        Setting::Ptr levelSetting = std::make_shared<Setting>();
        levelSetting->loadSetting(file::getPath({levelDirectory, "setting"}, StringPool.EXT_NERO),
                                  true,
                                  true);

        // Create Level Context
        GameLevel::Context levelContext(levelName, levelSetting, levelDirectory, nullptr, nullptr);

        // Create a new Level Builder
        m_LevelBuilder = std::make_shared<LevelBuilder>(levelContext);
        m_LevelBuilder->setEditorSetting(m_EditorSetting);
        m_LevelBuilder->setRenderContext(m_RenderContext);
        m_LevelBuilder->setRenderTexture(m_RenderTexture);
        m_LevelBuilder->loadResource();

        if(file::directoryEmpty(file::getPath({levelDirectory, "chunk"})))
        {
            auto worldChunk = m_LevelBuilder->addChunk();
            m_LevelBuilder->setSelectedChunk(worldChunk);

            m_LevelBuilder->saveGameLevel();
        }
        else
        {
            m_LevelBuilder->loadGameLevel();
        }
    }

    void AdvancedScene::closeSelectedLevel()
    {
        // TODO close selected level
    }

    void AdvancedScene::removeLevel(const std::string& levelName)
    {
        // TODO remove level
    }

    void AdvancedScene::registerLevel(const std::string& levelName)
    {
        // register level
        m_RegisteredLevelTable.push_back(levelName);

        Setting scene_document;
        scene_document.loadSetting(
            file::getPath({m_ProjectSetting->getString("project_directory"), "Scene", "scene"},
                          StringPool.EXT_NERO),
            true,
            true);
        scene_document.setStringTable("level_table", m_RegisteredLevelTable);
        file::saveFile(
            file::getPath({m_ProjectSetting->getString("project_directory"), "Scene", "scene"},
                          StringPool.EXT_NERO),
            scene_document.toString(),
            true);
    }

    void AdvancedScene::unregisterLevel(const std::string& levelName)
    {
        auto it =
            std::find(m_RegisteredLevelTable.begin(), m_RegisteredLevelTable.end(), levelName);

        if(it != m_RegisteredLevelTable.end())
            m_RegisteredLevelTable.erase(it);

        Setting scene_document;
        scene_document.loadSetting(
            file::getPath({m_ProjectSetting->getString("project_directory"), "Scene", "scene"},
                          StringPool.EXT_NERO),
            true,
            true);
        scene_document.setStringTable("level_table", m_RegisteredLevelTable);
        file::saveFile(
            file::getPath({m_ProjectSetting->getString("project_directory"), "Scene", "scene"},
                          StringPool.EXT_NERO),
            scene_document.toString(),
            true);
    }

    void AdvancedScene::createScreen(const Parameter& parameter)
    {
        /*auto gameScreenBuilder = std::make_shared<GameScreenBuilder>();

        return gameScreenBuilder;*/
    }

    void AdvancedScene::setEditorSetting(const Setting::Ptr& setting)
    {
        m_EditorSetting = setting;
    }

    void AdvancedScene::setProjectSetting(const Setting::Ptr& setting)
    {
        m_ProjectSetting = setting;
    }

    std::vector<std::string> AdvancedScene::getRegisteredLevelTable()
    {
        return m_RegisteredLevelTable;
    }

    void AdvancedScene::setRenderContext(const RenderContext::Ptr& renderContext)
    {
        m_RenderContext = renderContext;
    }

    void AdvancedScene::setRenderTexture(const std::shared_ptr<sf::RenderTexture>& renderTexture)
    {
        m_RenderTexture = renderTexture;
    }

    LevelBuilder::Ptr AdvancedScene::getLevelBuilder() const
    {
        return m_LevelBuilder;
    }

    void AdvancedScene::setGameScene(GameScene::Ptr gameScene)
    {
        m_GameScene = gameScene;
    }

    void AdvancedScene::handleEvent(const sf::Event& event)
    {
        if(m_GameScene)
            m_GameScene->handleEvent(event);
    }

    void AdvancedScene::update(const sf::Time& timeStep)
    {
        if(m_GameScene)
            m_GameScene->update(timeStep);
    }

    void AdvancedScene::render()
    {
        if(m_GameScene)
            m_GameScene->render();
    }

    void AdvancedScene::renderShape()
    {
        if(m_GameScene)
            m_GameScene->renderShape();
    }

    void AdvancedScene::clearGameSceneObject()
    {
        m_GameScene = nullptr;
        m_GameLevelMap.clear();
    }

    void AdvancedScene::registerLevelClass(const std::string levelName, GameLevel::Ptr gameLevel)
    {
        m_GameLevelMap.emplace(levelName, gameLevel);
    }

    void AdvancedScene::buildGameScene()
    {
        if(!m_GameScene || !m_LevelBuilder)
            return;

        const std::string levelName = m_LevelBuilder->getLevelName();
        if(m_GameLevelMap.count(levelName))
        {
            GameLevel::Ptr gameLevel = m_GameLevelMap.at(levelName);

            // m_LevelBuilder->buildLevel(currentGamelLevel->getLevelRoot());

            m_GameScene->setGameLevel(gameLevel);
        }
    }

} // namespace nero
