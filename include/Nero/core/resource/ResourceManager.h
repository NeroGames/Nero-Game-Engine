////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef RESOURCEMANAGER_H_INCLUDED
#define RESOURCEMANAGER_H_INCLUDED
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/resource/FontHolder.h>
#include <Nero/core/resource/SoundHolder.h>
#include <Nero/core/resource/MusicHolder.h>
#include <Nero/core/resource/ShaderHolder.h>
#include <Nero/core/resource/ScriptHolder.h>
#include <Nero/core/resource/TextureHolder.h>
#include <Nero/core/resource/AnimationHolder.h>
////////////////////////////////////////////////////////////
namespace nero
{
    struct ResourceManager
    {
        typedef std::shared_ptr<ResourceManager>    Ptr;

        //HolderList Nero Engine v1.0
        //Count : 7
		FontHolder          font;
        SoundHolder         sound;
        MusicHolder         music;
        ShaderHolder        shader;
        ScriptHolder        script;
        TextureHolder       texture;
		AnimationHolder     animation;
    };
}
#endif // RESOURCEMANAGER_H_INCLUDED

/**
    \class nero::ResourceManager
    \ingroup resource

    The nero::ResourceManager is the central class to manager all resource.
    The class store several resource holders that handle each type of resources

    - nero::FontHolder
    - nero::SoundHolder
    - nero::MusicHolder
    - nero::ShaderHolder
    - nero::ScriptHolder
    - nero::TextureHolder
    - nero::AnimationHolder

    Configuration file
    \file /config/resource-config.json

    Usage example:
    \code
        nero::ResourceManager  resourceManager;
        resourceManager.font.load();
    \endcode
*/
