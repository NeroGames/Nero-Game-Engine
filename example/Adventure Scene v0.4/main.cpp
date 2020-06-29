////////////////////////////////////////////////////////////
// Project Adventure Scene
// Copyright (c) 2019 sk-landry
////////////////////////////////////////////////////////////
// The Macro NERO_RENDER_ENGINE allows to run the DevEngine when CodeBlocks is in Debug Mode
// The Render Engine will be launched when CodeBlocks is in Release Mode
#ifndef NERO_RENDER_ENGINE
    #include <Nero/engine/DevEngine.h>
    #include "AdventureScene.h"


    int main()
    {
        nero::DevEngine engine(1305);
            engine.addScene<ng::AdventureScene>("Adventure Scene");
        engine.run();

        return 0;
    }

#else
    #include <Nero/engine/RenderEngine.h>
    #include "AdventureScene.h"
    #include "LoadingScreen.h"

    int main()
    {
        auto startupScreen = std::make_shared<ng::LoadingScreen>();

        nero::RenderEngine engine(startupScreen);
            engine.setScene<ng::AdventureScene>("Adventure Scene");
        engine.run();

        return 0;
    }

#endif // NERO_RENDER_ENGINE

