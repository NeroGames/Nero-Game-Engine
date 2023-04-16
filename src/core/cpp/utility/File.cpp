////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2023 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/utility/Logging.h>
#include <Nero/core/cpp/engine/EngineConstant.h>
#include <Nero/core/cpp/utility/File.h>
#include <Nero/core/cpp/utility/CommandLine.h>
// Cpp
#include <fstream>
#include <vector>
#ifdef NERO_OS_WINDOW
#include <shellapi.h>
#endif
////////////////////////////////////////////////////////////
namespace nero
{
    namespace file
    {
        std::string getPath(const std::vector<std::string>& list, const std::string& extension)
        {
            // Blank path
            if(list.empty())
                return StringPool.BLANK;

            // Build path
            std::string path = StringPool.BLANK;

            for(const std::string& name : list)
            {
                path += name + "/";
            }

            // Remove last slash
            path.pop_back();

            // Add extension
            if(extension != StringPool.BLANK)
            {
                path += extension;
            }

            return getPath(path);
        }

        std::string getPath(const std::string& path)
        {
            return boost::algorithm::replace_all_copy(path, "\\", "/");
        }

        std::string getPath(const std::initializer_list<const char*>& list,
                            const std::string&                        extension)
        {
            return getPath(std::vector<std::string>(list.begin(), list.end()), extension);
        }

        std::string getWindowsPath(const std::vector<std::string>& list,
                                   const std::string&              extension)
        {
            // Blank path
            if(list.empty())
                return StringPool.BLANK;

            // Build path
            std::string path = StringPool.BLANK;

            for(const std::string& name : list)
            {
                path += name + "\\";
            }

            // Remove last slash
            path.pop_back();

            // Add extension
            if(extension != StringPool.BLANK)
            {
                path += extension;
            }

            return getWindowsPath(path);
        }

        std::string getWindowsPath(const std::string& path)
        {
            return boost::algorithm::replace_all_copy(path, "/", "\\");
        }

        std::string getWindowsPath(const std::initializer_list<const char*>& list,
                                   const std::string&                        extension)
        {
            return getWindowsPath(std::vector<std::string>(list.begin(), list.end()), extension);
        }

        std::string escapeSpace(const std::string& path)
        {
            std::vector<std::string> table;
            std::string              separator = StringPool.BLANK;

            if(path.find("/") != std::string::npos)
            {
                table     = string::splitString(path, '/');
                separator = "/";
            }
            else if(path.find("\\") != std::string::npos)
            {
                table     = string::splitString(path, '\\');
                separator = "\\";
            }
            else
            {
                table.push_back(path);
            }

            std::string result = StringPool.BLANK;

            for(std::string word : table)
            {
                if(string::splitString(word, ' ').size() > 1)
                    result += "\"" + word + "\"" + separator;
                else
                    result += word + separator;
            }

            // Remove last slash (or backslash)
            result.pop_back();

            return result;
        }

        std::string escapeSlash(const std::string& path)
        {
            return boost::algorithm::replace_all_copy(path, "/", "//");
        }

        std::string escapeBackslash(const std::string& path)
        {
            return boost::algorithm::replace_all_copy(path, "\\", "\\\\");
        }

        std::string loadText(const std::string& file)
        {
            std::ifstream stream(file);

            if(!stream)
            {
                throw std::runtime_error("failed to load text from " + file);
            }

            std::stringstream buffer;
            buffer << stream.rdbuf();

            return buffer.str();
        }

        nlohmann::json loadJson(const std::string& path, const bool& path_has_extantion)
        {
            std::string file = path;

            if(!path_has_extantion)
            {
                file = file + StringPool.EXT_JSON;
            }

            std::ifstream stream(file);

            if(!stream.is_open())
            {
                // Nero_log("failed to load json file : " + file);
                // Nero_log("file not found : " + file);
                return nlohmann::json();
            }

            nlohmann::json json;
            stream >> json;
            stream.close();

            return json;
        }

        void createDirectory(const std::string& name)
        {
            using namespace std::experimental::filesystem;
            create_directory(path(name));
        }

        bool fileExist(const std::string& name)
        {
            using namespace std::experimental::filesystem;
            return exists(name);
        }

        bool directoryExist(const std::string& name)
        {
            using namespace std::experimental::filesystem;
            return is_directory(name);
        }

        void saveFile(const std::string& fileName, const std::string& content, bool overwrite)
        {
            if(fileExist(fileName) && !overwrite)
            {
                return;
            }

            std::ofstream stream(fileName);

            if(!stream.is_open())
            {
                // Nero_log("failed to save file : " + fileName);
                return;
            }

            stream << content;
            stream.close();
        }

        void saveFile(const std::string&   fileName,
                      const unsigned char* content,
                      const unsigned int   length,
                      bool                 overwrite)
        {
            if(fileExist(fileName) && !overwrite)
            {
                return;
            }

            std::ofstream stream(fileName, std::ios::binary);

            if(!stream.is_open())
            {
                // Nero_log("failed to save file : " + fileName);
                return;
            }

            stream.write(reinterpret_cast<const char*>(content),
                         static_cast<std::streamsize>(length));
            stream.close();
        }

        bool removeFile(const std::string& name)
        {
            if(!fileExist(name))
            {
                return false;
            }

            using namespace std::experimental::filesystem;
            remove(path(name));

            return true;
        }

        bool removeDirectory(const std::string& name, bool recursive)
        {
            if(!directoryExist(name))
                return true;

            using namespace std::experimental::filesystem;

            if(recursive)
            {
                const std::string cmd = std::string("rmdir /s /q ") + "\"" + name + "\"";
                system(cmd.c_str());
            }
            else
            {
                const std::string cmd = std::string("rmdir /q ") + "\"" + name + "\"";
                system(cmd.c_str());
            }

            return true;
        }

        std::string getCurrentPath()
        {
            return std::experimental::filesystem::current_path().string();
        }

        std::string removeFileExtension(const std::string& filename)
        {
            size_t lastdot = filename.find_last_of(".");

            if(lastdot == std::string::npos)
                return filename;

            return filename.substr(0, lastdot);
        }

        bool checkExtention(const std::string&              extention,
                            const std::vector<std::string>& extentionTable)
        {
            bool result = false;

            for(const std::string& ext : extentionTable)
            {
                if(extention == "." + ext || extention == "." + string::toUpper(ext))
                {
                    result = true;
                    break;
                }
            }

            return result;
        }

        std::string replaceExtension(const std::string& filename, const std::string& extension)
        {
            return removeFileExtension(filename) + "." + extension;
        }

        std::string getParentDirectory(const std::string& directory, const int& level)
        {
            std::experimental::filesystem::path path(directory);

            for(int i = 1; i <= level; i++)
            {
                path = path.parent_path();
            }

            return path.string();
        }

        std::string getFileName(const std::string& filename, bool withExtension)
        {
            std::experimental::filesystem::path path(filename);

            if(withExtension)
            {
                return path.filename().string();
            }

            return path.filename().stem().string();
        }

        bool copyFile(const std::string&        source,
                      const std::string&        destination,
                      const std::ios::openmode& readMode,
                      const std::ios::openmode& writeMode)
        {
            std::ifstream sourceStream(source, readMode);
            std::ofstream destinationStream(destination, writeMode);
            destinationStream << sourceStream.rdbuf();
        }

        bool directoryEmpty(const std::string& name)
        {
            using namespace std::experimental::filesystem;

            return is_empty(path(name));
        }
    } // namespace file
} // namespace nero
