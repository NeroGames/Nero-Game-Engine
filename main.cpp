////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//Nero
#include <Nero/editor/Editor.h>
//Example
#include <first_adventure/FirstAdventure.h>
#include <second_adventure/SecondAdventure.h>
////////////////////////////////////////////////////////////

int main()
{
    //instantiate the editor
    nero::Editor editor;

    //register scene
    editor.addScene<ng1::FirstAdventure>("The First Adventure");
    editor.addLuaScene<ng2::SecondAdventure>("The Second Adventure");

    //launch the editor
    editor.run();

    return 0;
}
