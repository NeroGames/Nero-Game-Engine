////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS///////////////////////////
// Nero
#include <Nero/editor/project/AdvancedScene.h>
#include <Nero/core/cpp/utility/Utility.h>
////////////////////////////////////////////////////////////
namespace nero
{
    AdvancedScene::AdvancedScene(Setting::Ptr projectSetting)
        : m_ProjectSetting(projectSetting)
        , m_RenderContext(nullptr)
        , m_RenderTexture(nullptr)
        , m_LevelBuilder(nullptr)
        , m_RegisteredLevelTable()
    {
    }

    void AdvancedScene::init()
    {
        // Retrieve teh list of Game Level
        Setting::Ptr sceneSetting = std::make_shared<Setting>();
        sceneSetting->loadSetting(
            file::getPath({m_ProjectSetting->getString("project_directory"), "Scene", "scene"},
                          StringPool.EXT_NERO),
            true,
            true);
        m_RegisteredLevelTable       = sceneSetting->getStringTable("level_table");

        // Create Scene context
        m_SceneContext.sceneName     = m_ProjectSetting->getString("project_name");
        m_SceneContext.camera        = m_EditorCamera;
        m_SceneContext.renderTexture = m_RenderTexture;
        m_SceneContext.setting       = sceneSetting;
        m_SceneContext.engineType    = GameScene::EngineType::EDITOR;
        m_SceneContext.platformType  = GameScene::PlatformType::WINDOWS;
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

        Setting resourceSetting;
        resourceSetting.loadSetting(file::getPath({"setting", "resource"}, StringPool.EXT_JSON),
                                    true,
                                    true);
        // Create Level Context
        GameLevel::Context levelContext;
        levelContext.levelName       = levelName;
        levelContext.levelDirectory  = levelDirectory;
        levelContext.levelSetting    = levelSetting;
        levelContext.camera          = m_EditorCamera;
        levelContext.renderTexture   = m_RenderTexture;
        levelContext.resourceManager = std::make_shared<ResourceManager>(resourceSetting);

        // Create a new Level Builder
        m_LevelBuilder               = std::make_shared<LevelBuilder>(levelContext);
        m_LevelBuilder->setRenderContext(m_RenderContext);
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

        Setting sceneDocument;
        sceneDocument.loadSetting(
            file::getPath({m_ProjectSetting->getString("project_directory"), "Scene", "scene"},
                          StringPool.EXT_NERO),
            true,
            true);
        sceneDocument.setStringTable("level_table", m_RegisteredLevelTable);
        file::saveFile(
            file::getPath({m_ProjectSetting->getString("project_directory"), "Scene", "scene"},
                          StringPool.EXT_NERO),
            sceneDocument.toString(),
            true);
    }

    void AdvancedScene::unregisterLevel(const std::string& levelName)
    {
        auto result =
            std::find(m_RegisteredLevelTable.begin(), m_RegisteredLevelTable.end(), levelName);

        if(result != m_RegisteredLevelTable.end())
            m_RegisteredLevelTable.erase(result);

        Setting sceneDocument;
        sceneDocument.loadSetting(
            file::getPath({m_ProjectSetting->getString("project_directory"), "Scene", "scene"},
                          StringPool.EXT_NERO),
            true,
            true);
        sceneDocument.setStringTable("level_table", m_RegisteredLevelTable);
        file::saveFile(
            file::getPath({m_ProjectSetting->getString("project_directory"), "Scene", "scene"},
                          StringPool.EXT_NERO),
            sceneDocument.toString(),
            true);
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

    void AdvancedScene::setEditorCamera(const AdvancedCamera::Ptr& editorCamera)
    {
        m_EditorCamera = editorCamera;
    }

    LevelBuilder::Ptr AdvancedScene::getLevelBuilder() const
    {
        return m_LevelBuilder;
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

    void AdvancedScene::clearLoadedObject()
    {
        m_GameScene = nullptr;
        m_CreateCppGameSceneCallback.clear();
        m_CreateCppGameLevelCallbackMap.clear();
    }

    void AdvancedScene::setCreateSceneCallback(CreateCppGameSceneCallback callback)
    {
        m_CreateCppGameSceneCallback = std::move(callback);
    }

    void AdvancedScene::registerCreateLevelCallback(const std::string          levelName,
                                                    CreateCppGameLevelCallback callback)
    {
        m_CreateCppGameLevelCallbackMap.emplace(levelName, callback);
    }

    void AdvancedScene::buildGameScene()
    {
        // Create Game Scene
        m_GameScene                 = m_CreateCppGameSceneCallback(m_SceneContext);

        const std::string levelName = m_LevelBuilder->getLevelName();
        if(m_CreateCppGameLevelCallbackMap.count(levelName))
        {
            // Create Game Level
            GameLevel::Ptr gameLevel =
                m_CreateCppGameLevelCallbackMap.at(levelName)(m_LevelBuilder->getLevelContext());

            // m_LevelBuilder->buildLevel(currentGamelLevel->getLevelRoot());

            m_GameScene->setGameLevel(gameLevel);
        }
    }

} // namespace nero
