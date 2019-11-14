////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 SANOU A. K. Landry
////////////////////////////////////////////////////////////
#ifndef STARTUPSCREEN_H
#define STARTUPSCREEN_H
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/core/resource/FontHolder.h>
#include <Nero/core/resource/TextureHolder.h>
#include <Nero/core/resource/AnimationHolder.h>
#include <Nero/core/resource/SoundHolder.h>
#include <Nero/core/resource/MusicHolder.h>
//SFML
#include <SFML/Graphics/RenderWindow.hpp>
//STD
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    class StartupScreen
    {
        public:
			typedef std::unique_ptr<StartupScreen> Ptr;

                                            StartupScreen();
            virtual                        ~StartupScreen();

		public: //method to override
            virtual void                    init()                                      = 0;
            virtual void                    handleEvent(sf::Event& event)               = 0;
            virtual void                    update(const sf::Time& timeStep)            = 0;
            virtual void                    render()                                    = 0;
			virtual const sf::Color         getCanvasColor()	const					= 0;
			virtual const float             getDuration()		const					= 0;

		public:
            void                            setRenderWindow(sf::RenderWindow* renderWindow);
			void							setFontHolder(FontHolder::Ptr fontHolder);
			void							setTextureHolder(TextureHolder::Ptr textureHolder);
			void							setAnimationHolder(AnimationHolder::Ptr animationHolder);
			void							setSoundHolder(SoundHolder::Ptr soundHolder);
			void							setMusicHolder(MusicHolder::Ptr musicHolder);

		private:
			virtual void					destroy();

        protected:
             sf::RenderWindow*              m_RenderWindow;
			 FontHolder::Ptr				m_FontHolder;
			 TextureHolder::Ptr				m_TextureHolder;
			 AnimationHolder::Ptr			m_AnimationHodler;
			 SoundHolder::Ptr				m_SoundHolder;
			 MusicHolder::Ptr				m_MusicHolder;
    };
}
#endif // STARTUPSCREEN_H
