#include <Nero/resource/ScriptHolder.h>
#include <Nero/utility/Utility.h>

namespace nero
{
    ScriptHolder::ScriptHolder()
    {
        m_Configuration = loadConfiguration(RESOURCE_CONFIGURATION)["script"];
    }

    void ScriptHolder::load()
    {
        nero_log("///////////////////////// LOADING SHADER ///////////////////");

        nero_log("////////////////////////////////////////////////////////////");

    }

}

