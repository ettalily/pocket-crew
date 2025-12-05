# Pocket Crew

**Unit Name:** FGCT4015 Fundamentals of Games Development

**Student Name:** Rösetta Kennedy

**Student ID:** 2418091

**Repository Link:** https://github.com/ettalily/pocket-crew

**Documentation Link:** https://github.com/ettalily/pocket-crew/blob/main/INSTALL.md

**Build Link:** https://github.com/ettalily/pocket-crew/releases/tag/Submission

**Video Demonstration Link:** https://youtu.be/uGnA6cjl57Y

<img src="https://github.com/ettalily/pocket-crew/blob/main/dev-commentary/ss1.png?raw=true">

*Fig 1. Kennedy, R. 2025. Pocket Crew Starting Area Screenshot*

## Abstract
The task for this project was to make a game project in the raylib software development framework around the theme "pocket" while fulfilling specific project requirements.

The goal for this project was to make a 3D-plaformer game with an on-rail camera and focus on momentum-based movement that felt good and responsive to control. My approach was to first experiment with the 3D libraries and tools included in raylib to gauge what scope I could realistically achieve in the timeframe provided, before then working on basic movement and levels, expanding my scope depending on how well and efficiently I implemented the features and mechanics that I had planned.

The game would be a simple platformer collectathon about collecting an army of bugs to help you build a boat in order to get off a stranded island and return home.

## Research

### Game Sources

#### Super Mario 64

A key game source for this project was Super Mario 64 (Nintendo EAD, 1996). Created by Nintendo, Super Mario 64 is a classic in the 3D-platformer genre and is responsible for many design decisions and trends that persist in modern games, even outside the platforming genre, and remains relevant through it's speedrunning and modding scene, being well regarded for it's weighty, expressive, and skillful movement. Being very momentum and physics based, this game and it's successes are a key place to borrow ideas from for this project. 

<img src="https://www.nintendo.com/eu/media/images/06_screenshots/games_5/virtual_console_wii_u_3/wiiuvc_supermario64/WiiUVC_SuperMario64_04.jpg">

*Fig 2. Nintendo. Super Mario 64 Screenshot.*

##### Take aways:
- Weighty, heavily momentum-based movement that requires time and space to build or reduce speed.
- Low aerial drift to make jumps committal and emphasise building, angling, and maintaining speed.
- Dive-esque mechanic that let's the player build lots of speed with the risk-reward of changing direction or stopping being difficult at high speed.
- Wall jump or environmental movement interaction to encourage and reward the player for considering and taking in their environment.
- Design areas with the camera in mind (as to avoid many of the issues the game's camera has in confined areas).

Notably this game also has many less successful elements, such as it's questionable camera and level design that has very low lows, two traits that I feel are refined significantly in later platforming games. However, this game acts as a great blueprint for designing engaging, appealing, and interesting movement within a 3D-platformer, as well as how this movement should interact with the environment. These are elements I have drawn from in my project.

#### Crash Bandicoot N. Sane Trilogy

Another key game research source for this project was the faithful remakes of the original three Crash Bandicoot games that make up the Crash Bandicoot N. Sane Trilogy (Vicarious Visions, 2017). The original versions of these games' more linear design and focus on collectables paved the way for lots of 3D platformers of it's era. These remakes are more refined and modern versions of these games, with nicer visuals and quality of life changes, however are still very faithful reditions of their originals, committing to the design choices those games had made.

<img src="https://www.crashbandicoot.com/content/dam/atvi/Crash/gallery/crash16.jpg">

*Fig 3. Vicarious Visions. 2017. Crash Bandicoot N. Sane Trilogy Screenshot.*

##### Take Aways
- Interesting and varied linear level designs with collectables to reward skilled players, adding skill expression.
- Snappy and responsive controls that allow for precise jumps while strongly rewarding good spacing and timing.

Some elements of these games' designs are less compatible with my project, however the feeling of the controls, the level design, and the emphasis on good timing are elements I have drawn from in my project. 

### Documentation Sources
I referenced the raylib cheatsheet (Ray, e.d.) regularly to learn about the functions avaliable within raylib and to understand their syntax and usage.

I also referenced an example implementation of lighting shaders within raylib (Camacho, C. 2019).

I also used W3Schools (Refsnes Data AS, 1998) as a reference and learning tool for C++ concepts and correct keyword usage.

## Implementation

### Process
Planning and idea generation were done in parts: I had the idea for a 3D platformer where you collect things and to make sure I could accomplish my goals without having to cut anything out, I would implement things as they came up in a very iterative way. The benefits of this are that it made it easy for me to let the project take me where it wanted to; letting me see what features would feel right or add something after I had implemented previous features and could check how the game felt; and also had the effect of letting me not cut things out if I struggled to implement something or ran out of time. For example, the dive mechanic came as a natural evolution of how the jumping and movement felt after I had implemented those mechanics and gotten them feeling responsive.

### New Approaches
As I was new to both C++, 3D development, as well as working with raylib, this project required a lot of new approaches, workflows, and tools. Learning these things usually happened through trying to implement something, not finding success, then having to problem solve and learn how to use features in new ways or how to use new tools in order to solve the problem. It required a very flexible workflow in order to make a game as ambitious as this one without the comforts I am used to from working with commercial game engines.

Implementing gravity and basic momentum-based movement code wasn't new to me as I had done it a lot in 2D and it is rather similar just with another horizontal dimension. However, this is my first time having implemented it in 3D and it required a bit of more thought about camera placement, as well as pretty much requiring analogue support to feel good (in my opinion; playing 3D platformers without analogue is like trying to speed type on a keyboard made of paper). The controller input code was rather trivial with the tools raylib has included, which was nice.

This is my first time having to write a 3D collision system. I have done similar things in 2D but having a third dimension and wanting arbitrary level model support rather than something more basic like axis-aligned boxes meant I had to take a whole new approach to collisions using raycasting that I was not familiar with and figure things out from scratch, piecing together concepts one at a time. The collision code only runs on nearby geometry, doing an AABB check with simple rectangles for nearby geometry before running raycast collision against it's mesh. I am pretty happy with the result for a first try. The collision code is contained within [`collision.cpp`](https://github.com/ettalily/pocket-crew/blob/main/src/collision.cpp) and is definitely too big to show here, so I'll link to it.

### Challenges

Many of the challenges came from being new to programming in C++ and 3D game development and conventions.

An early challenge was figuring out building with raylib libraries and using multiple files in C++, but a bit of help from W3Schools and some asking other people on my course quickly helped me figure that out, which lead to me writing my build script and build commands and instructions in [`INSTALL.md`](https://github.com/ettalily/pocket-crew/blob/main/INSTALL.md), that eventually lead to my interactive command line bash build script: [`build.bash`](https://github.com/ettalily/pocket-crew/blob/main/build.bash).

Learning how to use pointers and standard library tools such as vectors and ifstream/ofstream was an interesting challenge, but I eventually found ways to implement both properly: using vectors in my loading code and dialogue code, as well as reading and writing from files for the app's preferences.cfg file, as well as for loading dialogue into the character's respective dialogue vectors before the main loop runs. Writing this reading and writing code was really rewarding because it is a completely new concept for me, as someone who usually works on rather small coding projects or in engines that handle a lot of this for the developer.

> Iterates through each bug character and gives them their lines of dialogue line by line, with a space indicating moving to the next bug character.

```cpp
void DialogueInit() {
    speechBubbleDialogue = LoadTexture("assets/textures/speechbubble.png");

    // Loads dialogue lines.
    std::ifstream bugDialogue ("assets/text/bugDialogue");
    if (!bugDialogue.is_open()) {
        std::cout << "Unable to open assets/text/bugDialogue\n";
        closeGame = true;
        return;   
    }
    std::string currentLine;
    int currentBug = 0;
    while (std::getline(bugDialogue, currentLine)) {
        if (currentLine == "") {
            currentBug++;
        } else {
            bugCollectables[currentBug].dialogue.push_back(currentLine);
        }
    }
    bugDialogue.close();
    std::cout << "Successfully loaded dialogue.\n";
}
```

<img src="https://github.com/ettalily/pocket-crew/blob/main/dev-commentary/ss2.png?raw=true">

*Fig 4. Kennedy, R. 2025. Pocket Crew Conversation Screenshot*

The particle system was fun to think up and required a bit of thinking out of the box to get them working properly. Firstly, I knew I didn't want to use instantiation and probably didn't need many particles, so used objects that would just turn themselves off after their animation finished and then back on when they restarted. They also run a timer for their animations. However, the billboard function I used to cut my spritesheets for the particles used floating point values for the sprite sizes, and as a result had small bits of the sprite next to it showing when they shouldn't with the floating point value I had set, so I extended the sprite sheet slightly so that no pixels would border neighbouring tiles, which fixed the issue. I used the implementation method for the "Hi" text bubble above bugs.

> Draws the particle if it is active, then iterates the animation timer and changes the frame or deactivates the particle if it reaches the frame's hold time.
```cpp
void Particle::Draw() {
    if (!active) return;
    DrawBillboardRec(cam.camera, texture, Rectangle{(float)frameSize * currentFrame, (float)frameSize * currentFrame, (float)frameSize, (float)frameSize}, position - (Vector3){ 0.0f, 0.15f, 0.0f}, (Vector2){ 1.0f, 1.0f }, WHITE);
    
    if (pauseMenu) return;
    timer ++;
    if (timer != frameSpeed) return;
    timer = 0;
    currentFrame ++;
    if (currentFrame > frameCount) {
        currentFrame = 1;
        active = false;
    }
}
```

### Note on Implementation
Lots of my code is too big or complicated to cut down and summarise here; please skim through the scripts in `/src`, they are commented well, are quite readable, and give more insight into how things work than I can summarise here. This project got quite big, mostly due to 3D being complicated.

## User Testing

### Method
Testing was done with a mixed sample of peer and users not related to the course. Feedback was collected through a Google Forms questionnaire asking for grading on a variety of aspects of the game, as well as having open responses for bug reports, suggestions, or opinions on what works and doesn't work.

The key goals of testing were to figure out how the game felt to play in a variety of control types and user preferences and expectations for how the game would feel or should control.

### Results

#### Numerical Ratings

| Tester Type | Input Method | Movement/5 | Wall Jumping and Sliding/5 | Diving/5  | Sound/5 | Game Enjoyment/4 |
|----|----|----|----|----|----|----|
| User | Gamepad D-Pad | 4/5 | 3/5 | 3/5 | 3/5 | 4/4 |
| Peer | Gamepad Analogue | 5/5 | 4/5 | 3/5 | 3/5 | 4/4 |
| Peer | Gamepad Analogue | 4/5 | 3/5 | 4/5 | 2/5 | 3/4 |
| User | Keyboard WASD | 5/5 | 2/5 | 3/5 | 3/5 | 3/4 |

#### Text Responses Summaried
- The camera and it's angles and movement were a common issue, one which was a lot worse on non-analogue control schemes.
- Small collision bugs on ledges, and major collision bugs in very niche situations when interacting with odd geometry in cramped spaces.
- Wall sliding angles were too precise and were hard to get on non-analogue setups.
- Wall jump input felt odd to some users as you had to hold towards the wall prior to jumping.
- The sharp harsh lighting can make it hard to judge depth and distance in certain scenarios.

### Take Aways

What the user tests demonstrated were that the controls had been tweaked rather well, with a few adjustments for movement needed such as adding more leniency to wall jumping and sliding angles and timing, but overall the game felt good to control and move around in. It also showed that people found the concept of bug hunting fun and found the sound design passable, however had issues with the fast motions of the camera in places and the harsh lighting. The lighting isn't something I had the time nor knowledge of GLSL shader writing to do much about, but I did make changes to the camera and level layout to fix some of the camera troubles the testers had, as well as fixing some collision bugs.

## Outcome

### Final Build
Releases for linux and windows platforms can be found on the [GitHub Releases](https://github.com/ettalily/pocket-crew/releases/tag/Submission) page.

### Video Demonstration
[Pocket Crew Showcase](https://youtu.be/uGnA6cjl57Y).

*Fig 5. Kennedy, R. 2025. Pocket Crew Showcase Video*

## Critical Reflection
### Research Effectiveness
The research I did had a massive impact on the final project, massively influencing the design of the controls and the way the game feels to play. It also strongly influenced my level design.

### Positive Analysis
The games controls, movement, and physics feel nice and intuitive to control, and I managed to implement all of the features I had aimed to into the final build, including functional collisions with handling for ledges and slopes, controller support, model importing, dialogue, loading from text files, and particles.

### Negative Analysis
The game as it currently is is very barebones, having not much content, having very rudimentary graphics and lighting, and many unused systems such as other camera modes and level loading and unloading that would have been used in later areas if I had more time to work on the game. Additionally, the collisions can be buggy in certain worse case scenario geometry configurations. The code base is also a lot messier than I would have liked it to be, however it is acceptable for learning C++ while developing the game.

### Take Aways
Overall, this project has taught me the importance of a clean and scalable code base, and has taught me a lot of fundamentals of C++ and general project organisation. I enjoyed the flexibility working with a framework game me as making my own physics, collisions, animation handling, and text loading from scratch feels intuitive to me and let me learn more concepts and try my hand at more diverse programming challenges than I would have otherwise. This increased diversity of programming experience will hopefully aid in future projects. Additionally, I have also learnt the value of playtesting early, as many core changes I had made to the design of the controls, levels, and camera are direct responses to user playtesting feedback.

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

Refsnes Data AS (1998). "W3Schools" [Website]. Avaliable at: https://www.w3schools.com (Accessed 24/11/25).

Vicarious Visions (2017). *Crash Bandicoot N. Sane Trilogy* [Video Game]. Playstation 4. Activision.

# List of Illustrations

Fig 1. Kennedy, R. (2025). *Pocket Crew Starting Area Screenshot*.

Fig 2. Nintendo. *Super Mario 64 Screenshot*. Avaliable at: https://www.nintendo.com/eu/media/images/06_screenshots/games_5/virtual_console_wii_u_3/wiiuvc_supermario64/WiiUVC_SuperMario64_04.jpg (Accessed 1/12/25).

Fig 3. Vicarious Visions (2017). *Crash Bandicoot N. Sane Trilogy Screenshot.* Available at: https://www.crashbandicoot.com/nsane-trilogy/gallery/image-16 (Accessed 1/12/25).

Fig 4. Kennedy, R. (2025). *Pocket Crew Conversation Screenshot*.

Fig 5. Kennedy, R (2025). *Pocket Crew Showcase Video* [Video]. 