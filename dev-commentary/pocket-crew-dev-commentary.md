# Pocket Crew

**Unit Name:** FGCT4015 Fundamentals of Games Development

**Student Name:** Rösetta Kennedy

**Student ID:** 2418091

**Repository Link:** https://github.com/ettalily/pocket-crew

**Documentation Link:** https://github.com/ettalily/pocket-crew/blob/main/INSTALL.md

**Build Link:** https://github.com/ettalily/pocket-crew/releases

**Video Demonstration Link:** 

## Abstract
The task for this project was to make a game project in the raylib software development framework around the theme "pocket" while fulfilling specific project requirements.

The goal for this project was to make a 3D-plaformer game with an on-rail camera and focus on momentum-based movement that felt good and responsive to control. My approach was to first experiment with the 3D libraries and tools included in raylib to gauge what scope I could realistically achieve in the timeframe provided, before then working on basic movement and levels, expanding my scope depending on how well and efficiently I implemented the features and mechanics that I had planned.

The game would be a simple platformer collectathon about collecting an army of bugs to help you build a boat in order to get off a stranded island and return home.

## Research

### Game Sources

#### Super Mario 64

A key game source for this project was Super Mario 64 (Nintendo EAD, 1996). Created by Nintendo, Super Mario 64 is a classic in the 3D-platformer genre and is responsible for many design decisions and trends that persist in modern games, even outside the platforming genre, and remains relevant through it's speedrunning and modding scene, being well regarded for it's weighty, expressive, and skillful movement. Being very momentum and physics based, this game and it's successes are a key place to borrow ideas from for this project. 

<img src="https://www.nintendo.com/eu/media/images/06_screenshots/games_5/virtual_console_wii_u_3/wiiuvc_supermario64/WiiUVC_SuperMario64_04.jpg">

*Fig 1. Nintendo. Super Mario 64 Screenshot.*

##### Take aways:
- Weighty, heavily momentum-based movement that requires time and space to build or reduce speed.
- Low aerial drift to make jumps committal and emphasise building, angling, and maintaining speed.
- Dive-esque mechanic that let's the player build lots of speed with the risk-reward of changing direction or stopping being difficult at high speed.
- Wall jump or environmental movement interaction to encourage and reward the player for considering and taking in their environment.
- Design areas with the camera in mind (as to avoid many of the issues the game's camera has in confined areas).

Notably this game also has many less successful elements, such as it's questionable camera and level design that has very low lows, two traits that I feel are refined significantly in later platforming games. However, this game acts as a great blueprint for designing engaging, appealing, and interesting movement within a 3D-platformer, as well as how this movement should interact with the environment. These are elements I have drawn from in my project.

### Documentation Sources
I referenced the raylib cheatsheet (Ray, e.d.) regularly to learn about the functions avaliable within raylib and to understand their syntax and usage.

I also referenced an example implementation of lighting shaders within raylib (Camacho, C. 2019).

I also used W3Schools (Refsnes Data AS, 1998) as a reference and learning tool for C++ concepts and correct keyword usage.

## Implementation
### Process
### New Approaches
### Challenges

## User Testing

### Method
Testing was done with a mixed sample of peer and users not related to the course. Feedback was collected through a Google Forms questionnaire asking for grading on a variety of aspects of the game, as well as having open responses for bug reports, suggestions, or opinions on what works and doesn't work.

The key goals of testing were to figure out how the game felt to play in a variety of control types and user preferences and expectations for how the game would feel or should control.

### Results

What the user tests demonstrated were that the controls had been tweaked rather well, with a few adjustments for movement needed such as adding more leniency to wall jumping and sliding angles and timing, but overall the game felt good to control and move around in. It also showed that people found the concept of bug hunting fun and found the sound design passable, however had issues with the fast motions of the camera in places and the harsh lighting. The lighting isn't something I had the time nor knowledge of GLSL shader writing to do much about, but I did make changes to the camera and level layout to fix some of the camera troubles the testers had.

## Outcome

### Final Build
Releases for linux and windows platforms can be found on the [GitHub Releases](https://github.com/ettalily/pocket-crew/releases) page.

### Video Demonstration

## Critical Reflection
### Research Effectiveness
The research I did had a massive impact on the final project, massively influencing the design of the controls and the way the game feels to play. It also strongly influenced my level design.

### Positive Analysis
The games controls, movement, and physics feel nice and intuitive to control, and I managed to implement all of the features I had aimed to into the final build, including functional collisions with handling for ledges and slopes, model importing, dialogue, loading from text files, and particles.

### Negative Analysis
The game as it currently is is very barebones, having not much content, having very rudimentary graphics and lighting, and many unused systems such as other camera modes and level loading and unloading that would have been used in later areas if I had more time to work on the game. Additionally, the collisions can be buggy in certain worse case scenario geometry configurations. The code base is also a lot messier than I would have liked it to be, however it is acceptable for learning C++ while developing the game.

### Take Aways
Overall, this project has taught me the importance of a clean and scalable code base, and has taught me a lot of fundamentals of C++ and general project organisation. I enjoyed the flexibility working with a framework game me as making my own physics, collisions, animation handling, and text loading from scratch feels intuitive to me and let me learn more concepts and try my hand at more diverse programming challenges than I would have otherwise. This increased diversity of programming experience will hopefully aid in future projects. Additionally, I have also learnt the value of playtesting early, as many core changed I had made to the design of the controls, levels, and camera are direct responses to user playtesting feedback.

## Declared Assets
**Software Development Library:** raylib (Ray, 2013)

**IDE:** Visual Studio Code (Microsoft, 2015)

**3D Graphics Creation Software:** Blender (Blender Foundation, 1994)

**Texture and Graphic Creation Software:** Krita (Krita Foundation, 2005)

**Music Creation Software:** FL Studio 2025 (Image-Line Software, 1998)

**Sound Effect Creation Software:** jsfxr (Fredricksen, E. 2011)

**GLSL Lighting Shaders:** Chris Camacho's raylib lighting shader example (Camacho, C. 2019)

## Bibliography

Blender Foundation, and contributors. (1994). *Blender* [Software]. At: https://projects.blender.org/blender/blender.git (Accessed 24/11/25).

Camacho, C. (2019). *raylib [shaders] example - basic lighting*. At: https://www.raylib.com/examples/shaders/loader.html?name=shaders_basic_lighting (Accessed 24/11/25).

Fredricksen, E. and Pettersson, T. (2011). *jsfxr* [Software]. At: https://github.com/grumdrig/jsfxr (Accessed 24/11/25).

Image-Line Software. (1998). *FL Studio* [Software]. At: https://www.image-line.com/ (Accessed 24/11/25).

Krita Foundation, KDE, and contributors. (2005). *Krita* [Software]. At: https://github.com/KDE/krita (Accessed 24/11/25).

Microsoft (2015). *Visual Studio Code* [Software]. At: https://github.com/microsoft/vscode (Accessed  24/11/25). Microsoft.

Nintendo EAD (1996). *Super Mario 64* [Video Game]. Nintendo 64. Nintendo.

Ray (2013). *raylib* [Software]. At: https://github.com/raysan5/raylib (Accessed 24/11/25).

Ray (e.d.). *raylib cheatsheet*. At: https://www.raylib.com/cheatsheet/cheatsheet.html (Accessed 24/11/25).

Refsnes Data AS. 1998. "W3Schools" [Website]. Avaliable at: https://www.w3schools.com (Accessed 24/11/25).

# List of Illustrations

Fig 1. Nintendo. *Super Mario 64 Screenshot*. Avaliable at: https://www.nintendo.com/eu/media/images/06_screenshots/games_5/virtual_console_wii_u_3/wiiuvc_supermario64/WiiUVC_SuperMario64_04.jpg (Accessed 1/12/25).