# The Invasion

A classic arcade space shooter written from scratch in C++ with the
[Allegro 5](https://liballeg.org/) library, originally built during the
author's first semester of university.

This repository holds the object-oriented rewrite of that original code,
which also runs in the browser through WebAssembly.

---

## Gameplay

Survive endless waves of invading ships. Every kill raises your score, and
the fleet grows and toughens as you go. You have three lives.

| Key | Action |
| --- | --- |
| `W` `A` `S` `D` | Move the ship |
| `Space` | Fire |
| `Enter` / `Space` | Confirm in menus |
| `Esc` | Return to the main menu |

Your best score is stored between sessions — in a file on the desktop, and
in IndexedDB in the browser.

---

## Architecture

The original was a single 1,034-line `main()` with four nested `while`
loops and a set of free functions operating on plain structs. The rewrite
splits it along the single-responsibility principle: one reason to change
per class.

```
src/
├── main.cpp              Entry point
├── core/
│   ├── Config.h          World constants
│   ├── GameContext.h     Shared state handed to every scene
│   └── Game.{h,cpp}      Composition root, event loop, scene transitions
├── entities/
│   ├── Entity.{h,cpp}    Position, extent, symmetric AABB test
│   ├── Player.{h,cpp}    Player state and movement bounds
│   ├── Enemy.{h,cpp}     Enemy ship state and its hit rule
│   └── Projectile.{h,cpp}  A single shot, player or enemy
├── systems/
│   ├── Starfield.{h,cpp}      Three-layer parallax background
│   ├── EnemyFleet.{h,cpp}     Spawning, crowding, movement
│   ├── PlayerWeapon.{h,cpp}   Player guns and enemy hits
│   ├── EnemyWeapon.{h,cpp}    Enemy guns and player hits
│   ├── CombatResolver.{h,cpp} Shot-versus-shot interception
│   └── Hud.{h,cpp}            In-match interface
├── scenes/
│   ├── Scene.h                Screen contract
│   ├── MenuScene.{h,cpp}
│   ├── CreditsScene.{h,cpp}
│   ├── PlayScene.{h,cpp}
│   └── GameOverScene.{h,cpp}
└── platform/
    ├── Platform.{h,cpp}        Allegro lifecycle, window, event queue
    ├── ResourceLibrary.{h,cpp} RAII owner of every asset
    ├── AudioDirector.{h,cpp}   Music and sound effects
    └── HighScoreStore.{h,cpp}  Score persistence
```

Headers are included relative to `src/`, for example
`#include "systems/EnemyFleet.h"`.

### Design notes

**Scenes replace nested loops.** Each screen implements `Scene` and answers
only for itself; `Game` owns the single event loop and decides transitions.

**Ownership is explicit.** `ResourceLibrary` and `AudioDirector` are
non-copyable and release their handles in their destructors, replacing the
long manual teardown at the end of the original `main()`.

**Asymmetric collision rules stay where they belong.** The shared
`Entity::intersects` implements the symmetric bounding-box test. Enemy hit
detection and enemy crowding use different, lopsided formulas in the
original; those live in `Enemy` and `EnemyFleet` rather than being
flattened into a single misleading helper.

---

## Bugs fixed during the rewrite

| Problem | Effect |
| --- | --- |
| `Personagens.h` declared `Enimies`, the code used `Enemies` | The published source did not compile at all, on any compiler |
| `Powers missile[3]` initialised with 10 elements | Wrote 7 elements past the end of a stack buffer |
| `chance -= 50 * score / count / 10` with no floor | Reached zero at high scores, so `rand() % chance` divided by zero and crashed the game |
| Nine ship sprites reloaded per match, never freed | Leak that grew with every round played |
| Animation counter started at 9, outside the 0–8 draw range | Ship invisible for the first 20 frames of every match |
| `exposion_time` and `moving_timer` created but never used | Timers leaked; dead code |
| Header encoded as ISO-8859-1 while the source was UTF-8 | Rejected by stricter compilers than MSVC |

Gameplay behaviour is otherwise preserved deliberately, including quirks
such as a single shot being able to damage several overlapping enemies in
the same frame.

---

## Building

### Desktop (Linux)

Requires CMake, a C++17 compiler and Allegro 5.

```bash
sudo pacman -S cmake allegro          # Arch Linux
cd "The Invasion"
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./the_invasion
```

The binary is placed next to `Assets/`, `fonts/` and `sounds/`, because
asset paths are resolved relative to the working directory.

### Browser (WebAssembly)

Requires Emscripten.

```bash
sudo pacman -S emscripten             # Arch Linux
./web/build.sh
cd web/dist && python3 -m http.server 8080
```

Then open <http://localhost:8080>.

The build compiles Allegro 5 against its **SDL2 backend**, which is the
supported path to Emscripten — Allegro has no Emscripten backend of its
own. Three flags carry the port:

- `-sASYNCIFY` together with `-DALLEGRO_WAIT_EVENT_SLEEP=ON` lets
  `al_wait_for_event` yield to the browser, so the blocking event loop
  works unchanged.
- `-sUSE_VORBIS=1 -sUSE_OGG=1` provide the codecs for the `.ogg`
  soundtrack.
- `--preload-file` packs the assets into the virtual filesystem.

Browsers keep the audio context suspended until the first user gesture, so
the soundtrack starts on the first key press rather than on load.

---

## Credits

- Programming, game design: **Anderson Gonçalves**
- Art: **Anderson Gonçalves**, **Hugo City**
