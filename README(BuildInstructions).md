# Risk

Project Name : Risk

Group Number : 1F

Group Members : Rumeysa ÖZAYDIN Ahmet Serdar GÜRBÜZ Merve KILIÇARSLAN Osman Burak İNTİŞAH Elnur ALİYEV

Group Name : RedPill

Project Topic : Project is to convert the Risk board game into a digital game.

Risk game is strategy board game that can be played by 2 to 6 people. In the game there is a political map which shows divided territories.There is 6 continents (hence 6 players at most, every player starts with a continent) on the map. Players wait their turn to play, they try to conquer other players continents with their armies. Results of the soldier attacks are determined by dice rolls. In the game players can form alliances with each other however alliance does not mean two players can win the game since game's goal is to conquer every continent so that other players are eliminated from the game.

Requirements : There should be a map and 6 players for the basic game. Soldiers (real game has 3 types of soldiers) with a superiority level. 6 different kinds of these soldiers(can be seperated by colors or names or shapes) for 6 players. 52 cards with different challenges and advantages. 5 dice for attacks and turns. Other required specialities will be added according to the additions made troguhout the implementation.

Differences/Adittional Changes : The map of the game will be different so that the game can be played with a concept (ex. Game of Thrones etc.). Type of the attacks will be improved and attack rules will be changed. Original game has 3 types of soldier however more soldier types with different attack powers can be added. There can be a random attack where the numbers each player intended to roll will be determined by an external force. Other funtionalities can be added like "money" or "mines" for some territories that some territories have more advantage over others.

References : http://www.gamingcorner.nl/rules/boardgames/risk_uk.pdf

Build Instructions:

1. You can download our exe folder and open the game from exe file.

2.You can download our source code and open it from your IDE(sthe steps are explained accordingly to Visual Studio), the steps are: 

-Download the git repository

-Create an empty project in your IDE.

-Copy the folders you have downloaded into the folder for your empty project 

-Change the project property in C/C++ part. Add the include folder of sfml folder to “Additional Include Directories”

-In input part of the linker part, add the following dependencies to “Additional Dependencies”: sfml-main.lib, sfml-audio.lib, sfml-window.lib, sfml-graphics.lib, sfml-system.lib, sfml-network.lib

-In the general linker part, add the lib folder of sfml folder to “Additional Library Directories” 

-Build Your program in Release and x64

-try to run the program and create your relase folder (you should get errors for .dll files)

-Then copy the .dll files in the sfml/bin folder to "projectName/SolutionName/x64/Release"

-Also, copy the assets file to the same relase folder.

-Click run in your IDE.

