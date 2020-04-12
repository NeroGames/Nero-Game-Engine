////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef STRING_H
#define STRING_H
///////////////////////////HEADERS//////////////////////////
//Boost
#include <boost/algorithm/string.hpp>
//Cpp
#include <string>
#include <sstream>
#include <vector>
#include <functional>
////////////////////////////////////////////////////////////
namespace nero
{
	///\addtogroup string
	/// @{
	namespace string
	{
		const struct
		{
			//file extension
			const std::string EXT_JSON			= ".json";
			const std::string EXT_TEXT			= ".txt";
			const std::string EXT_LUA			= ".lua";
			const std::string EXT_DLL			= ".dll";
			const std::string EXT_H				= ".h";
			const std::string EXT_CPP			= ".cpp";
			const std::string EXT_NERO			= ".nero";
			const std::string EXT_INI			= ".ini";
			const std::string EXT_PNG			= ".png";
			//common character
			const std::string BLANK				= "";
			const std::string SPACE				= " ";
			const std::string NEW_LINE			= "\n";
			const std::string UNDERSCORE		= "_";
			const std::string SLASH				= "/";
			const std::string ANTI_SLASH		= "\\";
			//char
			const char		  SPACE_CHAR        = ' ';

		} StringPool;

		///\brief List of Formats
		enum class Format //Input = Nero Game Engine
		{
			SNAKE_CASE,			//Nero_Game_Engine
			SNAKE_CASE_LOWER,	//nero_game_engine
			SNAKE_CASE_UPPER,	//NERO_GAME_ENGINE
			CAMEL_CASE,			//neroGameEngine
			CAMEL_CASE_UPPER,   //NeroGameEngine
			COMPACT,			//Nerogameengine
			COMPACT_LOWER,		//nerogameengine
			COMPACT_UPPER,		//NEROGAMEENGINE
			UPPER_CASE,			//NERO GAME ENGINE
			LOWER_CASE,			//nero game engine
			TITLE_CASE,			//Nero Game Engine
			SENTENCE_CASE,		//Nero game engine
		};

		///\brief Convert string
		template <typename T>
		std::string						toString(T const& value);
		///\brief Format a string input
		std::string						formatString(const std::string& input, Format format = Format::SNAKE_CASE);
		std::string						wrapString(const std::string& message, size_t maxLetter);
		void							fillCharTable(const char** charTab, const std::vector<std::string>& stringTab);
		void							fillCharTable(char** charTab, const std::vector<std::string>& stringTab);
		void							fillCharArray(char* charArray, int arraySize, const std::string& string);
		char*							arrayFromVector(const std::vector<std::string>& stringTab);
		std::vector<std::string>		getWordTable(std::string string);
		std::vector<std::string>&		splitString(const std::string &s, char delim,std::vector<std::string> &elems);
		std::vector<std::string>		splitString(const std::string &s, char delim);
		std::string						escapeAntiSlash(const std::string& word);
		bool							matchPattern(const std::string& input, const std::string& pattern);

		template <typename T>
		std::string toString(T const& value)
		{
			std::stringstream stream;
			stream << value;
			return stream.str();
		}

	}///@}
}
////////////////////////////MACRO///////////////////////////
#define nero_fill_char_array(arrayTable, vectorTable) for(unsigned int i = 0; i < vectorTable.size(); i++ ) arrayTable[i] = vectorTable[i].c_str();
////////////////////////////////////////////////////////////
#endif // STRING_H
