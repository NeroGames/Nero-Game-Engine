////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
////////////////////////////////////////////////////////////
#ifndef RESOURCEHOLDER_H_INCLUDED
#define RESOURCEHOLDER_H_INCLUDED

///////////////////////////HEADERS//////////////////////////
//STD
#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>
//BOOST
#include <boost/filesystem.hpp>
////////////////////////////////////////////////////////////

namespace nero
{
    template <typename Resource>
    class ResourceHolder
    {
        public:
            void                load(const std::string& folder);

            template <typename Parameter>
            void                load(const std::string& file, const Parameter& secondParam);

            Resource&           get(std::string id);
            const Resource&     get(std::string id)  const;
            std::string         getLog()            const;



        private:
            void                insertResource(std::string id, std::unique_ptr<Resource> resource);


        private:
            std::map<std::string, std::unique_ptr<Resource>>	m_ResourceMap;
            std::string                                         m_logMessage;

    };

}

#include "ResourceHolder.inl"

#endif // RESOURCEHOLDER_H_INCLUDED
