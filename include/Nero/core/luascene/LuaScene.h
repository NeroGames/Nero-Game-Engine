/*#ifndef LUASCENE_H
#define LUASCENE_H
#include <Nero/core/scene/Scene.h>
#include <memory>
#include <lua/lua.hpp>
#include <sol/sol.hpp>

namespace  nero
{
    class LuaScene : public Scene
    {
		public:
			typedef std::shared_ptr<LuaScene> Ptr;

		public:
			LuaScene(Scene::Context context);
			std::string getLuaSceneFile();

			void initLuaScene();
			void loadLuaScene();

		private:
			void	bindLogging();
			void	bindScene();

		private:
			sol::state		m_LuaState;
			sol::table		m_NeroTable;
    };
}

#endif // LUASCENE_H*/
