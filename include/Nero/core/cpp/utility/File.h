////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef FILE_H
#define FILE_H
///////////////////////////HEADERS//////////////////////////
// Nero
#include <Nero/core/cpp/utility/String.h>
#include <Nero/core/cpp/utility/Platform.h>
// Json
#include <json/json.hpp>
// Cpp
#include <experimental/filesystem>
////////////////////////////////////////////////////////////
namespace nero
{
    namespace file
    {
        /// Convert path to Linux style path
        std::string    getPath(const std::string& path);
        /// Build path Linux style
        std::string    getPath(const std::initializer_list<const char*>& list,
                               const std::string&                        extension = StringPool.BLANK);
        std::string    getPath(const std::vector<std::string>& list,
                               const std::string&              extension = StringPool.BLANK);
        /// Convert path to Windows style path
        std::string    getWindowsPath(const std::string& path);
        /// Build path Windows style
        std::string    getWindowsPath(const std::initializer_list<const char*>& list,
                                      const std::string& extension = StringPool.BLANK);
        std::string    getWindowsPath(const std::vector<std::string>& list,
                                      const std::string&              extension = StringPool.BLANK);
        /// Espace space, add double quote
        std::string    escapeSpace(const std::string& path);
        /// Espace slash
        std::string    escapeSlash(const std::string& path);
        /// Espace backlash,
        std::string    escapeBackslash(const std::string& path);
        // get file content
        std::string    loadText(const std::string& file);
        // load Json file
        nlohmann::json loadJson(const std::string& path, const bool& path_has_extantion = false);
        // Create a directory if not exist
        void           createDirectory(const std::string& name);
        // Check if a file exist
        bool           fileExist(const std::string& name);
        // get directory or file name
        std::string    getFileName(const std::string& name, bool withExtension = false);
        // check if a directory exist
        bool           directoryExist(const std::string& name);
        // remove file
        bool           removeFile(const std::string& name);
        // remove directory
        bool           removeDirectory(const std::string& name, bool recursive = true);
        //
        bool           directoryEmpty(const std::string& name);
        // Save a text to a file
        void saveFile(const std::string& name, const std::string& content, bool overwrite = false);
        // Save binary date to a file
        void saveFile(const std::string&   name,
                      const unsigned char* content,
                      const unsigned int   length,
                      bool                 overwrite = false);
        // Current path
        std::string getCurrentPath();
        std::string removeFileExtension(const std::string& filename);
        bool        checkExtention(const std::string&              extention,
                                   const std::vector<std::string>& extentionTable);
        std::string replaceExtension(const std::string& filename, const std::string& extension);
        std::string getParentDirectory(const std::string& directory, const int& level = 1);
        // remove file
        bool        copyFile(const std::string&        source,
                             const std::string&        destination,
                             const std::ios::openmode& readMode  = std::ios::binary,
                             const std::ios::openmode& writeMode = std::ios::binary);
    } // namespace file
} // namespace nero
#endif // FILE_H
