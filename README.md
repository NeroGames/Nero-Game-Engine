
![alt text](https://www.nero-game.com/resource/artwork/github/github_readme_header.png)
![alt text](https://www.nero-game.com/resource/artwork/github/github_readme_engine_ui.png)
![alt text](https://www.nero-game.com/resource/artwork/github/github_readme_download.png)
## Project Nero Game Engine

The Nero Game Engine is an Advanced SFML Game Engine build arround Box2D. Its designed follows two rules : Simplicity and Intuitiveness. 
SFML (Simple and Fast Multimedia Library) is a cross-platform library designed to ease the development of games and multimedia applications.  Box2D is a 2D physics simulator engine. It can simulate the behavior of rigid bodies in a world affected by the gravity. By combining the power of those two libraries the Nero Game Engine offers a Interface that will speed you SFML games development workflow.

The Engine is composed of two main features : An Interface and an API.

###  The Engine Interface 
The Interface (image above) allows to build a Game World wihtout coding. The Interface load ressources like Sprite, and Animation automatically. You can then fill your Game World with various Objects.

### The Engine API

The API is build arrow a Class calls Scene. A Scene represent a Game, from a Scene you can retrieve any Object created using the Interface and manage their life time.

    #include <Nero/engine/DevEngine.h>
    
    //Forward declaration
    class NewScene;
    
    int main()
    {
	    nero_log("hello world");
    
        nero::DevEngine engine;
            engine.addScene<NewScene>("new scene");
        engine.run();
    
        return 0;
    }
    
    //Scene class
    class NewScene : public nero::Scene
    {
        public:
            NewScene(nero::Context context): nero::Scene(context)
            {
                //Ctr
            }
    };


## SFML Dev Blog

The Engine development is related on the SFML Forum. If you want a historic on the Engine development this is the place to go.

[Nero Game Engine Development Journey \[SFML Forum\]](https://en.sfml-dev.org/forums/index.php?topic=21019.0)

## Where to Start

If you are using the Windows OS,  the easiest way to start is to download the Engine SDK on the [website](https://www.nero-game.com) and then follows some tutorials. The SDK is built arround the MinGW compiler and the CodeBlocks IDE. It offers an already setup environment where you can start coding just after downloading.

In you want to work on another environement you can still build the Engine.

## Links
- [Website](https://www.nero-game.com/) 	Download the Engine and learn how to use it
- [Twitter](https://twitter.com/nerogameengine) Don't miss any news
- [Youtube]() Find some videos here. There may be some videos tutorials in the future
- [Forum](https://www.nero-game.com/forum/) Ask any questions here

## How to Build

#### Dependencies 

| Library | Type | Sources | 
| --- | --- | --- |
| SFML 2.4.1 | built | [github](https://github.com/SFML/SFML) |
| SFGUI 0.3.1 | built | [github](https://github.com/TankOs/SFGUI) |
| Box2D 2.3.0| built | [github](https://github.com/erincatto/Box2D) |
| Easyoggingpp | header only| [github](https://github.com/zuhd-org/easyloggingpp) |
| Json|  header only | [github](https://github.com/nlohmann/json) |
| Sol2 | header only | [github](https://github.com/ThePhD/sol2) |
| Imgui| header only | [github](https://github.com/ocornut/imgui) |
| Imgui-sfml| header only | [github](https://github.com/eliasdaler/imgui-sfml) |
| Sqlite | header only | [website](https://www.sqlite.org/index.html) |
| Lua| built | [website](https://www.lua.org/download.html) |
| Boost 1.61.0 (system & filesystem) | built | [website](https://www.boost.org/users/history/version_1_61_0.html) |

#### Cmake GUI

The Engine relies on Cmake GUI for for the build. Download Cmake [here](https://cmake.org)

## Support the Project

If you like the project give a star and share with your friends. 

