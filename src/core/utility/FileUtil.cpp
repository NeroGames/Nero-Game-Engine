#include <Nero/core/utility/FileUtil.h>
#include <Nero/core/utility/LogUtil.h>
#include <Nero/core/engine/EngineConstant.h>
#include <fstream>
#include <vector>
#include <experimental/filesystem>

namespace nero
{
    std::string getPath(const std::vector<std::string>& list, const std::string& extansion)
    {
        if(list.size() == 0)
        {
            throw std::runtime_error("empty path");
        }

        std::string path = StringPool.BLANK;

        //build path Window OS
        for(const std::string& name : list)
        {
            path += name + "/";
        }

        path.pop_back();

        if(extansion != StringPool.BLANK)
        {
            path += extansion;
        }

        #ifdef NERO_OS_LINUX
            path = "./" + path;
        #endif

           //nero_log(path);

        return path;
    }

    std::string loadText(const std::string& file)
    {
        std::ifstream stream(file);

        if(!stream)
        {
            throw std::runtime_error("failed to load text from " + file);
        }

        std::string text("");
        std::string word;

        while(stream >> word)
        {
            if(word == "\\n")
            {
                text += "\n";

            }
            else
            {
                text += word + " ";
            }
        }

        return text;
    }

    nlohmann::json loadJson(const std::string& path, const bool& path_has_extantion)
    {
        std::string file  = path;

        if(!path_has_extantion)
        {
            file  = file + StringPool.EXTANSION_JSON;
        }

        std::ifstream stream(file);

        if(!stream.is_open())
        {
            //nero_log("failed to load json file : " + file);
            //nero_log("file not found : " + file);
            return  nlohmann::json();
        }

        nlohmann::json json;
        stream >> json;
        stream.close();

        return json;
    }

    nlohmann::json loadSetting(const std::string& name)
    {
        return loadJson(getPath({EngineConstant.FOLDER_SETTING, name}));
    }

    void createDirectory(const std::string& name)
    {
        using namespace  std::experimental::filesystem;
        create_directory(path(name));
    }

    bool fileExist(const std::string& name)
    {
        using namespace  std::experimental::filesystem;
        return exists(name);
    }

    bool directoryExist(const std::string& name)
    {
        using namespace  std::experimental::filesystem;
        return is_directory(name);
    }

    void  saveFile(const std::string& fileName, const std::string& content, bool overwrite)
    {
        if(fileExist(fileName) && !overwrite)
        {
            return;
        }


        std::ofstream stream(fileName);

        if(!stream.is_open())
        {
            //nero_log("failed to save file : " + fileName);
            return;
        }

        stream << content;
        stream.close();
    }

    void  saveFile(const std::string& fileName, const unsigned char* content, const unsigned int length, bool overwrite)
    {
        if(fileExist(fileName) && !overwrite)
        {
            return;
        }

        std::ofstream stream(fileName, std::ios::binary);

        if(!stream.is_open())
        {
            //nero_log("failed to save file : " + fileName);
            return;
        }

        stream.write(reinterpret_cast<const char*>(content), static_cast<std::streamsize>(length));
        stream.close();
    }

    bool removeFile(const std::string& name)
    {
        if(!fileExist(name))
        {
            return false;
        }

        using namespace  std::experimental::filesystem;
        remove(path(name));

        return true;
    }

    bool removeDirectory(const std::string& name, bool recursive)
    {
        using namespace  std::experimental::filesystem;
        remove(path(name));

        return true;
    }

    std::string getCurrentPath()
    {
        using namespace  std::experimental::filesystem;

        return getCurrentPath();
    }


}






