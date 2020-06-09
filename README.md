# UnrealSumo Plugin for UE4

### Introduction
This plugin is an interface to simulate traffic between [SUMO](http://sumo.dlr.de/wiki/Sumo_at_a_Glance)  and Unreal Engine 4 environment. SUMO is an open resource to generate microscopic road traffic simulation with large road networks. 
However, SUMO allows user to extract simple 2D traffic simulation information through TraCI. SUMO only provides limited sensing information in the complex driving scenario, whereas the interface developed in this repository between Unreal Engine and SUMO allows the information extraction from the more commonly used sensors, such as Lidar and Camera.
UnrealSumo plugin built on [Unreal Egnine](https://www.unrealengine.com/en-US/) that offers physically and visually realistic SUMO road traffic simulation. It is desgined from the ground up to be extensible to acocommodate new types of vehicles. Current version of plugin is tested with Unreal Engine 4.22.


### Plugin Setup
Unreal Engine and SUMO need to be installed before setup the plugin.

#### 1.	SUMO Traffic Setup
Before set up the Unreal Engine project, SUMO (Simulation of Urban MObility) must be installed and SUMO simulation scenarios need to be prepared. Within the SUMO configuration file, we can set the remote port number that bind to the socket. 
(Graph)
#### 2.	Create Project with Unreal Engine
Unreal Engine provides detailed setup in this [link](https://docs.unrealengine.com/en-US/Engine/Basics/Projects/Browser/index.html).
#### 3.	Add UnrealSumo Plugin
Download the repository and copy a UnrealSumo Plugin folder under project plugin repository. 
(Graph)

### Test UnrealSumo.uplugin


### Furtherwork
Upcoming features for the next step is to synchronize traffic light and predestrain behavior between SUMO and Unreal Engine. 


