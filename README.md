
![alt text](https://www.nero-game.com/resource/artwork/github/github_readme_header_02.png)
![alt text](https://www.nero-game.com/resource/artwork/github/github_readme_engine_v1.png) 
## Nero Game Engine r1.0.5

The Nero Game Engine is an Advanced SFML Game Engine built around Box2D. Its design follows two rules : Simplicity and Intuitiveness. <br> 
SFML (Simple and Fast Multimedia Library) is a cross-platform library designed to ease the development of games and multimedia applications.  Box2D is a 2D physics engine. It can simulate the behavior of rigid bodies in a world affected by gravity. <br>
By combining the power of those two libraries the Nero Game Engine offers an interface that will speed up your SFML game development workflow.

The Engine is composed of two main components : An Interface and an API.

###  The Engine Interface 

The Engine Interface (image above) allows you to build a Game World without coding. The Engine Interface loads resources like Sprites, Animations, Sounds, and Music automatically. You can then fill your Game World with various Games Objects. The Interface also allows you to create Game Screens for the different menus of your Game.

### The Engine API

The Engine API is built around a Class called **nero::Scene**. The Scene represents your Game. From your Game Scene, you can retrieve all your Games Objects created using the Engine Interface and then manage their lifetime.

```cpp
#include <Nero/engine/DevEngine.h>

//Forward declaration
class MyScene;

int main()
{
    nero_log("hello world");
    
    nero::DevEngine engine(1305);
        engine.addScene<MyScene>("new scene");
    engine.run();

    return 0;
}

//Scene class
class MyScene: public nero::Scene
{
	public:
	    MyScene(nero::Scene::Context context): nero::Scene(context)
	    {
		//Ctr
	    }
};
```

## SFML Dev Blog

The Engine Development is related to the SFML Forum. If you want a history of the Engine Development this is the place to go.

[Follow the Engine Development Journey \[SFML Forum\]](https://en.sfml-dev.org/forums/index.php?topic=21019.0)

## Where to Start
If you are using the Windows OS,  the easiest way to start is to download the Engine SDK on the [Website Download Page](https://www.nero-game.com) and then follow some tutorials. The SDK is built around the **MinGW compiler** and the **CodeBlocks IDE**. It offers an already setup environment where you can start coding just after downloading.

If you want to work in another environment you can still build the Engine.

## Links
- [Website](https://www.nero-game.com/) 	Download the Engine and learn how to use it
- [Twitter](https://twitter.com/nerogameengine) Don't miss any news
- [Youtube](https://www.youtube.com/channel/UCIR0nDUMGzJ3Pkpa3gDCUeQ?view_as=subscriber) Find some videos tutorial here
- [Forum](https://www.nero-game.com/forum/) Ask any questions here


## Support the Project

If you like the Project give a star and share it with your friends.  You can also join our Team. Whether you are a video game enthusiast, a developer, a game artist, a web designer, or a content writer, you are welcome. If you have any suggestions or just want to say hi! feel free to leave a tweet.

## Release Notes

[Checkout the Engine Release Notes here](./Release.md)


## How to Build

### Cmake GUI

The Engine relies on Cmake GUI for for the build. Download Cmake [here](https://cmake.org)

### Dependencies 

| Library | Type | Sources | 
| --- | --- | --- |
| SFML 2.4.1 | built | [github](https://github.com/SFML/SFML) |
| SFGUI 0.3.1 | built | [github](https://github.com/TankOs/SFGUI) |
| Box2D 2.3.0| built | [github](https://github.com/erincatto/Box2D) |
| Lua| built | [website](https://www.lua.org/download.html) |
| Boost 1.61.0 (system & filesystem) | built | [website](https://www.boost.org/users/history/version_1_61_0.html) |
| Easyoggingpp | header only| [github](https://github.com/zuhd-org/easyloggingpp) |
| Json|  header only | [github](https://github.com/nlohmann/json) |
| Sol2 | header only | [github](https://github.com/ThePhD/sol2) |
| Imgui| header only | [github](https://github.com/ocornut/imgui) |
| Imgui-sfml| header only | [github](https://github.com/eliasdaler/imgui-sfml) |
| Sqlite | header only | [website](https://www.sqlite.org/index.html) |



