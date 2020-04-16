////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/cpp/utility/File.h>
#include <Nero/core/cpp/engine/EngineConstant.h>
#include <Nero/core/cpp/utility/Logging.h>
//Cpp
#include <fstream>
#include <vector>
////////////////////////////////////////////////////////////
namespace nero
{
	namespace file
	{
		std::string getPath(const std::vector<std::string>& list, const std::string& extension)
		{
			#ifdef NERO_OS_WINDOW
				return getWindowsPath(list, extension);
			#endif

			#ifdef NERO_OS_LINUX
				return getLinuxPath(list, extension);
			#endif
		}



		std::string getWindowsPath(const std::vector<std::string>& list, const std::string& extansion)
		{
			if(list.empty())
			{
				return StringPool.BLANK;
			}


			std::string path = StringPool.BLANK;

			for(const std::string& name : list)
			{
				path += getWindowsPath(name) + "\\";
			}

			path.pop_back();

			if(extansion != StringPool.BLANK)
			{
				path += extansion;
			}

			return path;
		}

		std::string getWindowsPath(const std::string& path, bool escapeSpace)
		{
			//replace all slash by a double back_slash
			std::string temp =  boost::algorithm::replace_all_copy(path, "/", "\\");

			std::string result = StringPool.BLANK;

			//add double quote if needed
			if(escapeSpace)
			{
				std::vector<std::string> table = string::splitString(temp, '\\');
				for(std::string word : table)
				{
					if(string::splitString(word, ' ').size() > 1)
						result += "\"" + word + "\"\\";
					else
						result += word + "\\";
				}

				result.pop_back();
			}
			else
			{
				result = temp;
			}

			return result;
		}

		std::string getLinuxPath(const std::string& path)
		{
			//replace all slash by a double back_slash
			return boost::algorithm::replace_all_copy(path, "\\", "/");
		}



		std::string getLinuxPath(const std::vector<std::string>& list, const std::string& extansion)
		{
			//TODO get linux path
			 return StringPool.BLANK;
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
			std::string file  = path;

			if(!path_has_extantion)
			{
				file  = file + StringPool.EXT_JSON;
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
			//TODO
			return nlohmann::json();
			//return loadJson(getPath({EngineConstant.FOLDER_SETTING, name}));
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

			if(recursive)
			{
				std::error_code error;
				remove_all(path(name), error);
				nero_log(error.message());
			}
			else
			{
				return remove(path(name));
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

		   if (lastdot == std::string::npos)
			   return filename;

		   return filename.substr(0, lastdot);
	   }

		bool checkExtention(const std::string& extention, const std::vector<std::string>& extentionTable)
		{
		   bool result = false;

		   for (const std::string& ext : extentionTable)
		   {
			   if(extention == "." + ext)
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

		bool copyFile(const std::string& source, const std::string& destination)
		{
			std::ifstream  sourceStream(source, std::ios::binary);
			std::ofstream  destinationStream(destination,   std::ios::binary);

			destinationStream << sourceStream.rdbuf();
		}
	}
}






