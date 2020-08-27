#### Release 1.0.5 [Engine Update]
	- [Bug Fix]
		- in method nero::ObjectManager::createJoint, in the case of wheel-joint, fix property localAxisA 
		- in method nero::ObjectManager::findChildObject, check if input object is nullptr
	- [Enhancement]
		- remove "const" qualifier from method nero::Scene::getContext
		- rename method nero::Scene::enableFollowTarget to nero::Scene::enableCameraTarget
		- rename method nero::Scene::updateTargetOffset to nero::Scene::setCameraTargetOffset
		- add new method nero::Scene::getCamera
		- rename method nero::Collision::isCollising to nero::Collision::isColliding
		- rename method nero::Collision::isObjectCollising to nero::Collision::isObjectColliding
		- add new method nero::ObjectManager::getNewJointId
		- move method nero::Scene::enableLayer to nero::ObjectManager::enableLayer
		- move method nero::Scene::disableLayer to nero::ObjectManager::disableLayer
		- rename all joint property maxMotorForce to maxMotorTorque
		- rename method nero::Scene::followTarget to nero::Scene::followCameraTarget and make it overridable (virtual void)

#### Release 1.0.4 [Engine Update]
	- [Regression Fix]
		- bring back update 1.0.1 and 1.0.2
		- create resource/script and resource/shader folder at startup 

#### Release 1.0.3 [Engine Update]
	- [Enhancement] 
		- Make DevEngine Window size, adjustable
		
#### Release 1.0.2 [Engine Update]
	- [Bug Fix] Load Sprite Sheet
		- fix sprite sheet loading in TextureHolder.cpp
	- [Enhancement]
		- remove scene auto saving log message
	- [Other]
		- remove automatique log configuration

#### Release 1.0.1 [Engine Update]

	- [Bug Fix] Scene Renderer window resolution
		- remove method nero::Scene::setResolution
		- remove method nero::Scene::getResolution
		- add 	 method nero::Scene::getSceneResolution	
		- update CodeBlock Project Wizard
		- update Adventure Scene example

	- [Enhancement]
		- change render canvas width to 720 pixels
		- update engine interface size 

#### Release 1.0.0 [Initial Release]
	
	- None
