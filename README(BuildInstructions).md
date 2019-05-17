# risk-final
1. You can download our exe folder and open the game from exe file.

2.You can download our source code and open it from your IDE(sthe steps are explained accordingly to Visual Studio), for this the steps are: 

-Download the git repository

-Create an empty project with the name Risk in your IDE.

-Copy the folders you have downloaded into the folder for your empty project 

-Change the project property in C/C++ part. Add the include folder of sfml folder to “Additional Include Directories”

-In input part of the linker part, add the following dependencies to “Additional Dependencies”: sfml-main.lib, sfml-audio.lib, sfml-window.lib, sfml-graphics.lib, sfml-system.lib, sfml-network.lib

-In the general linker part, add the lib folder of sfml folder to “Additional Library Directories” 

-Build Your program in Release and x64

-try to run the program and create yorus relase folder (you should get errors for .dll files)

-Then copy the .dll files in the sfml/bin folder to "projectName/SolutionName/x64/Release"

-Also, copy the assets file to the same relase folder.

-Click run in your IDE.

