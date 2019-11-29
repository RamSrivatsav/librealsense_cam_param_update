# Intel RealSense T256 camera parameter updator.
Author: RG

Folder: RealSense-T256-camara-parameter-updater

Updated on: 28/11/2019
## Requirements:
1. Latest version of librealsense or >=v2.27.0.
## Instructions:
1. Extract Folder from git.
### if you are updating the camera with new intrinsics follow bellow steps:
2. Open the file rs-parameter-update.c inside the RealSense-T256-camara-parameter-updater folder and edit the "allocation of new intrinsics values" section according to your need. (Initially this code is configured to set and validate the existing configuration values for testing purpose)
3. After editing, run below command to create an executable file with name *out*.
```bash
gcc <./path_to_file> -o out -lrealsense2 -lGL -lglut -lGLU
```
4. After executabe is generated, run the code using below command.
```bash
./out
``` 
### To test the code follow bellow steps:
2. Change your directory into the RealSense-T256-camara-parameter-updater folder.
3. Test the code using below command.
```bash
./out
``` 
*the Initial Intrinsics and after modification should be the same for the test to be successful*
 


