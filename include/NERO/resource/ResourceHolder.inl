////////////////////////////////////////////////////////////
//
// Nero Game Engine
// Author : SANOU A. K. Landry
//
////////////////////////////////////////////////////////////
#ifndef RESOURCEHOLDER_INL_INCLUDED
#define RESOURCEHOLDER_INL_INCLUDED


namespace nero
{
    std::vector<std::string> splitString(const std::string &s, char delim);
}

namespace nero
{
    template <typename Resource>
    void ResourceHolder<Resource>::load(const std::string& folder)
    {
        using namespace boost::filesystem;
        path p(folder);
        m_logMessage = "\nResource_Holder-->Loading resource from \"" + p.string() +"\"";

        //Iterate over files in the folder
        directory_iterator it{p};
        while (it != directory_iterator{})
        {
            //When we found a Resource
            //if(it->path().extension().string())
            //{
                //Load the Resource
                std::unique_ptr<Resource> resource(new Resource());

                if (!resource->loadFromFile(it->path().string()))
                    throw std::runtime_error("Resource_Holder::load - Failed to load " + it->path().filename().string());

                //get the texture in m_TextureMap
                std::string resourceId = it->path().filename().stem().string();
                m_logMessage += "\nResource_Holder-->Resource loaded : \"" + resourceId + "\"";

                //Insert the texture in m_TextureMap
                insertResource(resourceId, std::move(resource));
            //}

            it++;
        }
    }

    template <typename Resource>
    template <typename Parameter>
    void ResourceHolder<Resource>::load(const std::string& file, const Parameter& secondParam)
    {
        // Create and load resource
        std::unique_ptr<Resource> resource(new Resource());
        if (!resource->loadFromFile(file, secondParam))
            throw std::runtime_error("ResourceHolder::load - Failed to load " + file);

        std::vector<std::string> split = splitString(file, '.');
        std::string resourceId = split[0];
        // If loading successful, insert resource to map
        insertResource(resourceId, std::move(resource));

        m_logMessage += "\nResource_Holder-->New resource loading from \"" + file +"\"";
    }

    template <typename Resource>
    Resource& ResourceHolder<Resource>::get(std::string id)
    {
        auto found = m_ResourceMap.find(id);
        assert(found != m_ResourceMap.end());

        return *found->second;
    }

    template <typename Resource>
    const Resource& ResourceHolder<Resource>::get(std::string id) const
    {
        auto found = m_ResourceMap.find(id);
        assert(found != m_ResourceMap.end());

        return *found->second;

        return m_ResourceMap[0];
    }

    template <typename Resource>
    void ResourceHolder<Resource>::insertResource(std::string id, std::unique_ptr<Resource> resource)
    {
        // Insert and check success
        auto inserted = m_ResourceMap.insert(std::make_pair(id, std::move(resource)));
        assert(inserted.second);
    }

    template <typename Resource>
    std::string ResourceHolder<Resource>::getLog() const
    {
        return m_logMessage;
    }
}

#endif // RESOURCEHOLDER_INL_INCLUDED
