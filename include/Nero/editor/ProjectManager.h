#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <memory>
#include <json/json.hpp>
#include <Nero/editor/GameProject.h>
#include <Nero/core/cpp/scene/Scene.h>
#include <Nero/core/lua/scene/LuaScene.h>
#include <boost/function.hpp>
#include <Nero/core/cpp/utility/StringUtil.h>
#include <Nero/core/cpp/utility/LogUtil.h>
#include <Nero/core/cpp/engine/Setting.h>
#include <Nero/editor/AdvancedCamera.h>

namespace  nero
{
    class ProjectManager
    {
        public:
			typedef std::unique_ptr<ProjectManager> Ptr;
			typedef std::shared_ptr<RenderContext> RenderContextPtr;
			typedef std::shared_ptr<sf::RenderTexture> RenderTexturePtr;
			typedef Scene::Ptr (CreateCppSceneFn)(Scene::Context);
			//typedef LuaScene::Ptr (CreateLuaSceneFn)(Scene::Context);

            enum Project_Type {LUA_PROJECT, CPP_PROJECT, CPP_LUA_PROJECT};

        public:
            ProjectManager();

		   void createProject(const Setting& parameter, int& status);
		   void createWorkspace(const Setting& parameter);
		   void importWorkspace(const std::string& directory);

		   bool isProjectExist(const std::string& projectName);
		   bool isWorksapceExist(const std::string& projectName);

		   void setSetting(const Setting::Ptr& setting);

           void loadAllProject();

           const std::vector<nlohmann::json>& getProjectTable() const;

           const std::vector<nlohmann::json> getWorkspaceProjectTable(const std::string& workpsace);


           const nlohmann::json getWorkspaceTable() const;
           const std::vector<std::string> getWorkspaceNameTable() const;

           const nlohmann::json findWorkspace(const std::string& name) const;

           void compileProject(const std::string& projectDirectory = StringPool.BLANK);
           void editProject();

           void loadLibrary();
		   void close();

            std::string exec(const char* cmd);
            std::string formatSceneClassName(std::vector<std::string> wordTable);
            std::string formatHeaderGard(std::vector<std::string> wordTable);
            std::string formatCmakeProjectName(std::vector<std::string> wordTable);
            std::string formatCmakeProjectLibrary(std::vector<std::string> wordTable);


            std::string getEngineDirectory() const;

			GameProject::Ptr openProject(const std::string& projectPath);
            nlohmann::json findProject(const std::string& workspace_name, const std::string& project_name);

            void openEditor(std::string cmake_project);

			void createCppProject(const Setting& parameter, int& status);
			void createLuaProject(const Setting& parameter, int& status);
			void createCppLuaProject(const Setting& parameter, int& status);

			void updateRecentProject(const std::string& projectDirectory);

			std::string getProjectDirectory(const Setting& parameter);

			void setRenderTexture(const RenderTexturePtr& renderTexture);
			void setResourceManager(const ResourceManager::Ptr& resourceManager);
			void setCamera(const Camera::Ptr& camera);

			void setRenderContext(const RenderContextPtr& renderContext);




        private:
           nlohmann::json      m_EditorSetting;
           std::vector<nlohmann::json>  m_ProjectTable;

           std::string m_EditorPid;

           GameProject::Ptr m_GameProject;

           boost::function<CreateCppSceneFn> m_CreateCppSceneFn;
		   //boost::function<CreateLuaSceneFn> m_CreateLuaSceneFn;
		   Scene::Ptr					m_GameScene;
		   Setting::Ptr					m_Setting;

		   RenderTexturePtr				m_RenderTexture;
		   Camera::Ptr					m_Camera;
		   ResourceManager::Ptr			m_ResourceManager;
		   RenderContextPtr				m_RenderContext;

    };
}



#endif // PROJECTMANAGER_H
