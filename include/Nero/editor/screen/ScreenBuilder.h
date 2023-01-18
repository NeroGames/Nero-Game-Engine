////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
////////////////////////////////////////////////////////////
#ifndef GAMESCREENBUILDER_H
#define GAMESCREENBUILDER_H
///////////////////////////HEADERS//////////////////////////
// STD
#include <memory>
////////////////////////////////////////////////////////////
namespace nero
{
    class ScreenBuilder {
      public:
        typedef std::shared_ptr<ScreenBuilder> Ptr;

      public:
        ScreenBuilder();
    };
} // namespace nero

#endif // GAMESCREENBUILDER_H
