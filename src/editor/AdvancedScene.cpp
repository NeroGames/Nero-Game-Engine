#include <Nero/editor/AdvancedScene.h>

namespace nero
{
    ////////////////////////////////////////////////////////////
   //DestructionListener
   void DestructionListener::SayGoodbye(b2Fixture* fixture)
   {
       B2_NOT_USED(fixture);
   }

   void DestructionListener::SayGoodbye(b2Joint* joint)
   {
       if (scene->m_MouseJoint == joint)
       {
           scene->m_MouseJoint = nullptr;
       }
       else
       {
           //scene->jointDestroyed(joint);
       }
   }

    AdvancedScene::AdvancedScene()
    {

    }
}

