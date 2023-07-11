Nikita Zyrianov's submission for Advanced Programming module Week 2 Coursework
-
Synopsis
=
This project was created using the Visual Studio open source software that
includes loads of prebuilt libraries specifically designed to create desktop
applications that include a GUI (a windowed application). This is how I attempted
to solve this challenge in C++. I understand that other programming languages have
a friendlier learning curve and will let me meet the criteria of this challenge
quicker but I wish to challenge myself and see what I could accomplish in the
3 weeks given to do this challenge. Unfortanetly, due to work commitments/deadlines,
I was not able to get this prototype to the level of quality that I would have
liked but ultimately this has been a great learning experience as I learnt how to
effectively manage my project, plan out what needs to be achieved, break down the
problem with abstraction to make the work more managable, and also with the time 
constraints; learning which feature is more valuable to have as opposed to other
features.
#
Running Adaship
=
In the git repository, there should be an adaship.exe ready to run. I am not sure 
if this will work on a different OS but I believe as long as you have Visual C++ 
Redistibutable installed on your machine it should work fine. If all else fails 
I would attempt to try and running it in the visual studio ide but you will still 
be able to see how the program is meant to run in the video :)
#
Challenge Outline
=

Overview
-
The challenge was to create a complex object-oriented program using an agile 
methodology that incorporates/blends in these aspects: \
• Problem Outline and Analysis. \
• Task or story backlog with clear definitions of done. \
• Evaluated Requirements reviewed and linked to required features and functions. \
• Use and incorporation of networking capability. \
• UML overall logic and/or data diagram. \
• Code Smells, reuse / reusable. \
• Fundamental OO: classes, objects, instantiation. \
• AdvancedOO: inheritance, composition, encapsulation, polymorphism, abstraction. \
• Team supported code reviews. \
• Code refactoring and/or use of design patterns. \
• Use of multithreading and/or asynchronous operations. \
• Designed and Implemented Security. \
• 15 Point Good Quality Standards Referenced Review. \
• Approach to design and development review. \
• Evidence of testing. \
• Further development/improvements Review. \
• Self-reflection and review on skills, experience and insights utilised and improved. \
\
The criteria gave an option of designing whatever game you want to or the option of 
recreating the battleship game. I ultimately chose to do the battleship game as it 
had a set of criteria already written out which would make my development journey for 
me simpler as I can just tick off each criteria as it gets implemented.
#
Planning
-
To begin designing the project I had to start planning out how the game would function
from start to finish. I planned it out in a flowchart for how the game would work just 
with an ai opponent and this the diagram I made:
![DiagramOfInitialDesign](diagram.png)  
\
As you can see, I decided to split the program up into 3 parts; a setup class, a main 
game class, and a determine winner class which are all unique in their own way.
\
\
The setup class is intended to run everytime the game starts and to fetch the default 
settings. If the settings file doesn't exist then a new one will be created for the user.
This is intended to let the user have control of how they want the board and the length of
the ships they would like. Once those settings are extracted, two cell grids are created to
simulate two player boards with then the enemy ships placed randomly and the player ships. 
With the alpha release, I thought it would best to leave the player randomly placing their 
ships as opposed to them selecting and placing but I let the player choose whether they are
happy with their ship placement or not. If not, then the ships will be randomly placed again
giving the player the option to either confirm their ship placement or to rearrange randomly
again.
\
\
The main game class is essentially a massive while loop that is ran until the last class kills
the program. It starts with a player selecting a cell on their grid, and then a check is ran if 
that cell has an enemy ship on it then the cell state will change to hit and paint the cell red, 
otherwise the cell state will be miss and coloured grey. Then, an enemy turn is processed, which
means that a cell is randomly selected and if hit, then will turn red and if missed will also turn
grey. Once enemy turn is processed, it goes back to the players turn. This is repeated until all 
ship cells are sunk.
\
\
The determine winner class is pretty self explanatory. After ever 'missile' fire, the class will 
check the board that has just been fired at and check to see if there are any ship cells left on it. If 
not, then a winner is announced depending on which board has no ship cells left.
#
Analysis & Decomposition
-
The startegy I took to solve this problem is TDD (Test driven development). This means I would 
write code that I know would fail but have the idea of how to solve a problem, then write the 
bare minimum to get that bit of code to work, and then refactor it all to be better written and 
more usable across my program. This type of development method ensures that the code you write is
of a higher standard as you test as you see any flaws in your code prior to your final 
version of it.
\
When beginning this project, it seemed a little overwhelming of where to start but breaking down 
that diagram into different epics would help prioritise what needs to be done first before something
else. The epics I broke out from the diagram are as follows: \
• Genterate Board \
• Make the cell grid clickable \
• Generate player and enemy ships \
• Process a hit/miss shot on board \
• Display user as winner when no more ship cells remain \
• Add in enemy shots to board \
• Display user as loser if enemy shoots down all player ship cells \
• Give user choice to reroll ship placement \
• Generate different sized ships for both boards \
• Create main menu screen with start and quit buttons \
\
The order I also completed these epics are the same as the order they are given above
as that makes the most sense to do it with. The first thing a player playing should see
is the grids, then the ships, shoot and fire, hit or miss, win or lose. This follows along
the process of abstraction as here we are breaking apart this large game into smaller 
components which is what ultimately makes up the game.
#
Development
=
Coding Standards
-
Adopting and using good coding standards is important when writing/releasing code however
it can also be difficult to maintain during the inital writing process. This is why I 
adopted TDD so that I was able to deliver a prototype of what this project could be first
and then go back and refactor my code to of a higher quality. I have seperated operations 
out into different methods whilst maintaining the 3 different classes. I have tried to avoid
code duplication where possible and to make the files themselves easier to read, I have split
them out per class. Any constants used are declared in the header files along with method names
and I have tried to not use global variables and keep variables local and only as parameters.
I have not made use of encapsulation or overloading due to lack of experience with it but I have
tried to keep my code neat and tidy whilst also maintaing an object-oriented programming (OOP)
style.
#
Pre-Alpha release
-
The pre-alpha release of the game was the bare bones of the game and what would only allow the
user to play the game against a computer and a winner is determined by who removes all the other 
player's ship cells first. There was no main menu as that was not necessary to the game and there
wasn't any varying ship lengths as I didn't create the function to read from the settings file yet.
As said before, the main goal of this release was to get the game working however it could and then
in future releases to work more effieciently. This meant that a lot of code in my next release will
be from this release. I had to create the generation of the window, the grids, random placement of 
ships, processing player clicking on grid cell, highlighting a hit or miss, process a computer shot,
update enemy board as well, check for winners, and announce winner.
The actual code quality for this release was very poor as it was ~500 lines of code all in one file broken apart into functions but 
with inconsistenet naming and formatting which I would have to revisit in my next iteration. The 
functionality of the code itself was ok but still had a few bugs such as ships spawning with 
intersecting one another or clicking outside the grid will also count as a turn. I made a note of
all these flaws with the program when going towards the alpha release of the game.
#
Alpha release
-
This is the current state that the game is at which I would describe as playable and enjoyable to an
extent.
\
The main goal of this release was to refactor all the code I had down to be more readable, consistent
with what I wanted, and spread out between different classes which is what I managed to achieve. I had 
also addressed the bugs in my program previously so that now clicks only on the player board are 
processed. I also created a function that will check for intersections and change the placement of 
the ships. I added in the main menu screen which is a basic display with two buttons to start the 
game and exit it respectively. I also added in the reading from a config file to get the ship lengths
and of how many ships will be in the game. Due to this being added, I decided to include the confi.ini 
file in the repository as part of the program however if it gets corrupted or deleted, there is a function
I made to recreate it with the default settings again. What the user can do with config file is add more ships
or less ships by adding and removing entries under the [Ships] object. Originally, I was going to use a 
predefiend method in WinApi to read the ini file however due to technical incompatibilites, I chose to 
use the fstream headers that comes with C++ and make it very specific.
\
The quality of code, I would say, has greatly improved since the last release. I created three seperate header
files; main, set up, and determine winner and with them I initialized all their components in there publicly
and moved all the relevant function into the relavant source file i.e. if a function will not be run more than
once or throughout the game then its most likely in the setup file. This let me have more readable code and 
give people that will look into the functions a better idea of what each one does. I put in comments as well to
help. When it came to testing for bugs in this release, I wanted to ensure that none would come from the settings
file so when it came to extracting information from there; if it wasn't a valid number written like the rest of 
the default values, it will not be placed on the board. If no value is valid, then a default set is given and config
file is remade for next time. This ensures that any same problems with the config file will
not persist in the next time the game is ran. Originally, I was going to have the board size as one of the config
options however I found that difficult to do due to the board size being needed to create the window before the 
settings is extracted.
\
If I was to do a new release, I would have liked to do a redesign on the window and grid generation as they are 
woven together at the moment but the window size should never really change whilst the grid should be able to go up
to 80x80, which it technically can but depending on your screen resoulution you are unlikely to see it. Another
feature I would consider adding is an easy/hard mode for the computer shot as currently it randomly fires wherever
but if it was more realistic it would try to along the x/y axis close to ship cell its hit. The reason I didn't add
this in the alpha build was that I believed it would be too hard to beat the computer currently with how ship cells 
looks on the board as you may not be able to tell where the whole ships is or when you have sunk a whole ship. That 
would also need to be another feature that would have to be added, an indication of when a ship has been sunk. There
were other requirements in the brief such as a fire button, the user being able to select and choose where they want
to place their ship. I would also want add an additional button on the main menu for the settings where the user 
can click into and edit the settings properly without needing to sift through any directory. Those are all the 
features I would have liked to include in my Pre-Beta release.
#
Evaluation
=
Looking back at my diagram ![DiagramOfInitialDesign](diagram.png) I have done most things the way I initially listed 
them here, especially with how the code is broken apart but if I was to do this diagram diffferently I wouldn't do the 
determine winner as its own seperate class but rather split the set up class even more.
Maybe into a create windows and menus class and extract settings and generate everything class. The reason I say this 
is that the SetUp source file is still over 300 lines of code whilst the DetermineWinner source file is less than 40. 
Another thing I would change is not being so strict in following how that diagram is. An agile workflow encourages an
evolving model and work to be introduced/done in sprints which I was doing to an extent but sticking too close to 
a diagram that was made before the work began isn't always the best idea as it doesn't take into account any new 
features that might need to be added in or unprecedented blockers that might show up during devolpment which might 
only be overcome by going about the problem with a different solution.
\
\
Overall, I believe I made a good attempt at creating Adaship and learnt a lot from the experience such as using 
decomposition to break problems apart, planning out each step of the user journey, using abstraction for user turns
to see whats the minimum work needed to achieve a turn based system. I hope this markdown has been insightful in the 
journey that was taken to create Adaship. Thank you.
#