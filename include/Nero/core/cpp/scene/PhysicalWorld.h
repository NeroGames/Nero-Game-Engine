////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2021 Sanou A. K. Landry
/////////////////////////////////////////////////////////////
#ifndef PHYSICALWORLD_H
#define PHYSICALWORLD_H
///////////////////////////HEADERS///////////////////////////
// STD
#include <memory>
/////////////////////////////////////////////////////////////
namespace nero
{
    class PhysicalWorld
    {
      public:
        // type definiton
        typedef std::shared_ptr<PhysicalWorld> Ptr;

      public:
        PhysicalWorld();
    };
} // namespace nero

#endif // PHYSICALWORLD_H
