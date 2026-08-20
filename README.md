**About**

*Wau Bulan Rising* is a physics-based vertical scroller where players steer a traditional Malaysian kite (Wau Bulan) while keeping a dangling passenger safe from mid-air hazards. The game is a love letter to Malaysia Day, celebrating local heritage through its mechanics and art. Players must navigate past falling durians, soaring hornbills, and bouncing gasings (spinning tops). Collecting the national flower (Bunga Raya) provides altitude boosts, pushing players upward past the Twin Towers to a triumphant "Merdeka!" victory under a starry night sky.

**Build & Run Instructions**

This project is built and executed exclusively via WSL (Windows Subsystem for Linux). It utilizes Raylib (v5.0 or newer).

1. Ensure you have WSL installed and running (WSL2 is recommended as it includes WSLg for out-of-the-box GUI/audio support).
2. You will need a C++ compiler and make installed within your Linux distribution (e.g., sudo apt install build-essential)
3. Open your WSL terminal, navigate to the root directory of the project, and run the command: 
    *make*
4. Once compiled, run the generated Linux binary directly in your WSL terminal to launch the game: 
    *./game*

**Architecture: Raylib**

I chose *Raylib* because it is a lightweight, code-first framework. It perfectly balances giving you low-level control over your architecture while handling the tedious platform-specific backend.

What *Raylib* Handles: Window lifecycle, OpenGL rendering, hardware-accelerated drawing (DrawTexturePro), input polling, audio streaming, and basic collision math (CheckCollisionCircles).

**Week 1 C++ Concepts Map**

Here is a roadmap of how fundamental C++ principles were applied to the game's architecture:

- *Const Correctness*: Applied to read-only references in game loops (const auto& obs) and immutable file paths (const char* texturePath).
- *Ownership & RAII*: std::unique_ptr manages obstacle lifetimes to ensure memory is automatically freed when hazards leave the screen, while manual RAII-like Unload() methods guarantee GPU texture cleanup.
- *Class Design*: Heavy use of inheritance and polymorphism. The abstract Obstacle base class dictates the contract for virtual methods like Update() and Draw(), which are uniquely implemented by GapObstacle or BouncingObstacle.
- *Containers*: std::vector dynamically tracks the active pool of polymorphic obstacles and handles the weighted randomization pools.

**Attributions:**

https://creazilla.com/media/clipart/4516/durian
<a href='https://pngtree.com/freepng/fresh-pandan-leaves-isolated-on_23174968.html'>png image from pngtree.com/</a>
<a href='https://pngtree.com/freepng/hibiscus-flower-in-cartoon-style-illustration-vector_23864028.html'>png image from pngtree.com/</a>
<a href="https://breakingcopyright.com/song/fredji-flying-high">Flying High</a> by Fredji | Free To Use YouTube license | <a href="youtube-free">License</a>

