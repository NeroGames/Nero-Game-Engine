/*#include <Nero/core/lua/scene/LuaScene.h>
#include <Nero/core/cpp/utility/StringUtil.h>
#include <Nero/core/cpp/utility/LogUtil.h>


namespace  nero
{
	LuaScene::LuaScene(Scene::Context context):
		 Scene (context)
		,m_LuaState()
    {
		m_LuaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::table, sol::lib::string);

		//m_LuaState


    }

	void LuaScene::initLuaScene()
	{
			m_LuaState.create_table("nero");
			m_NeroTable = m_LuaState["nero"];

			bindLogging();



			m_NeroTable["scene"] = LuaScene::Ptr(this);
			//m_LuaState.new_usertype<LuaScene>("LuaScene", "getName", &LuaScene::getName);
	}

	void LuaScene::bindLogging()
	{
		//
		m_NeroTable["logging"] = m_LuaState.create_table("logging");

		m_NeroTable["logging"]["info"]		= LOG_LEVEL::INFO;
		m_NeroTable["logging"]["warning"]	= LOG_LEVEL::WARNING;
		m_NeroTable["logging"]["debug"]		= LOG_LEVEL::DEBUG;

		m_NeroTable.set_function("log", [](const std::string& message)
		{
			nero_log(message);
		});

		m_NeroTable.set_function("log_if", [](const std::string& message, bool condition)
		{
			nero_log_if(message, condition);
		});

		m_NeroTable.set_function("log", [](const std::string& message, int level)
		{
			nero_log(message, level);
		});

		m_NeroTable.set_function("log_if", [](const std::string& message, bool condition, int level)
		{
			nero_log_if(message, condition, level);
		});

	}

	void LuaScene::bindScene()
	{

	}

	void LuaScene::loadLuaScene()
	{
		m_LuaState.script_file("template/lua-project/scene.lua");

	}

	std::string LuaScene::getLuaSceneFile()
	{
		return StringPool.BLANK;
	}


}*/
