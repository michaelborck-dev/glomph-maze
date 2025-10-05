# Audio Options Discussion - Detailed Analysis

## Why You Don't Hear Beeps Currently

**Terminal beep is disabled by default on most modern systems:**
- macOS: System Preferences → Sound → "Play sound on startup/alert" (often off)
- Linux: `xset b off` commonly run in .bashrc
- Many terminal emulators (iTerm2, Alacritty, etc.) disable beeps by default
- Even when enabled, it's often routed to system alert sound (jarring)

**This is why beep-only audio is problematic in 2025.**

---

## SDL vs miniaudio - Which Should We Use?

### SDL (Simple DirectMedia Layer)

**What it is:**
- Industry-standard multimedia library since 1998
- Used by: Steam games, Valve Source engine, many indie games
- Current version: SDL3 (2024), SDL2 still widely used

**Advantages:**
✓ **Very well tested** - battle-hardened in thousands of games
✓ **Excellent cross-platform** - Windows, Mac, Linux, BSD, mobile
✓ **Good documentation** - huge community, tons of tutorials
✓ **Active development** - SDL3 just released (2024)
✓ **Already has code** - assets/sounds/ has .mid/.xm files ready to play
✓ **Package managers** - `brew install sdl2 sdl2_mixer` (easy install)

**Disadvantages:**
✗ **Large dependency** - ~1.5MB library (but that's tiny by 2025 standards)
✗ **CMake complexity** - need to find SDL2, SDL2_mixer packages
✗ **Potential ncurses conflicts** - SDL can steal input/display (but SDL_mixer alone is safe)

**Real-world usage:**
- Steam: 50,000+ games use SDL
- Very common on developer machines
- Standard library in most Linux distros

---

### miniaudio

**What it is:**
- Single-header C library for audio playback
- Created by David Reid (drifting pixels)
- Public domain (no licensing concerns)

**Advantages:**
✓ **Zero dependencies** - just drop miniaudio.h in your project
✓ **Small** - ~70KB single header file
✓ **Modern** - actively developed (last update: months ago)
✓ **Cross-platform** - uses OS-native APIs (CoreAudio, WASAPI, ALSA)
✓ **Simple API** - easier than SDL for basic playback
✓ **No library installation** - just `#include "miniaudio.h"`

**Disadvantages:**
✗ **Can't play .mid/.xm files** - needs WAV, MP3, FLAC, OGG
✗ **Need to convert assets** - current sounds are MIDI/tracker formats
✗ **Smaller community** - less StackOverflow help than SDL
✗ **Header-only bloat** - ~70KB added to each .c file that includes it

---

## Why PortAudio is "Overkill"

PortAudio is **not** overkill because it's a terminal game - it's overkill because:

1. **Professional audio API** - designed for DAWs, audio editors, recording software
2. **Low-latency focus** - we don't need sub-5ms latency for Pac-Man beeps
3. **Callback-based** - requires setting up audio streams, buffers, callbacks
4. **More complex** - SDL/miniaudio hide this complexity for game audio

**Analogy:** Using PortAudio for game beeps is like using PostgreSQL to store a todo list.

---

## Current Assets Analysis

```bash
$ ls assets/sounds/
bonus.mid        ghost.mid        pellet.mid       siren1_up.mid
credit.mid       intermission.mid siren0_down.mid  siren2_down.mid
dot.mid          level.mid        siren0_up.mid    siren2_up.mid
dot2.mid         life.mid         siren1_down.mid  start.mid
dying.mid        fruit.mid                         start.xm
```

**All MIDI (.mid) or tracker (.xm) formats** - designed for SDL_mixer!

To use miniaudio, we'd need to:
1. Convert all to WAV/OGG (lossy process for MIDI)
2. Increase asset size (WAV files are larger)
3. Lose musical composition advantages of MIDI

---

## Recommendations by Priority

### OPTION 1: Add SDL2_mixer (RECOMMENDED)

**Best fit because:**
1. ✓ Assets are already in SDL-compatible formats
2. ✓ Code already written (just need to enable it)
3. ✓ Most common on developer machines
4. ✓ Easy to make optional (CMake option)
5. ✓ Standard solution for this use case

**Implementation:**
```cmake
option(ENABLE_AUDIO "Enable SDL audio support" ON)

if(ENABLE_AUDIO)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    target_compile_definitions(glomph PRIVATE USE_SDL_MIXER=1)
    target_link_libraries(glomph ${SDL2_LIBRARIES} ${SDL2_MIXER_LIBRARIES})
else()
    # Use beep (default)
endif()
```

**User experience:**
- `cmake -DENABLE_AUDIO=ON` → builds with SDL audio
- `cmake -DENABLE_AUDIO=OFF` → builds with beep/silent (default for simplicity)

---

### OPTION 2: Add miniaudio (If avoiding SDL)

**Good if:**
- You want zero external dependencies
- You're willing to convert assets to WAV/OGG
- You want single-header simplicity

**Implementation effort:** Medium
- Drop miniaudio.h in include/
- Convert all .mid files to .wav/.ogg (can use ffmpeg/timidity)
- Replace gamesfx() SDL code with miniaudio API

**Downsides:**
- Lose MIDI file benefits (small size, musical expressiveness)
- Larger git repo (WAV files are bigger)
- More work than just enabling existing SDL code

---

### OPTION 3: Keep beep + document audio is optional

**Current state:**
- Works everywhere (when beep enabled)
- Zero dependencies
- Retro authentic feel
- Can add audio later without breaking anything

**Best if:**
- You want to finish refactoring first
- You prioritize zero dependencies
- You don't mind no audio for now

---

## My Recommendation: SDL2_mixer with CMake Option

**Why:**
1. **Leverage existing work** - code + assets already there
2. **Industry standard** - SDL is the de facto game audio library
3. **Optional** - default to beep, enable audio with `-DENABLE_AUDIO=ON`
4. **Easy install** - `brew install sdl2 sdl2_mixer` on Mac
5. **Future-proof** - SDL3 just released, very active project

**Risk assessment:**
- **Low risk** - SDL_mixer doesn't interfere with ncurses (no video/input)
- **Isolated** - already wrapped in `#if USE_SDL_MIXER`
- **Optional** - can build without it
- **Reversible** - easy to disable if problems

---

## Implementation Strategy (if we go SDL route)

### Phase 1: Add CMake support (5 minutes)
```cmake
option(ENABLE_AUDIO "Enable SDL audio (requires SDL2_mixer)" OFF)
# Keep OFF by default for minimal deps
```

### Phase 2: Test on your machine (2 minutes)
```bash
brew install sdl2 sdl2_mixer
cmake -B build -DENABLE_AUDIO=ON
cmake --build build
./build/glomph
# Press S to toggle sound, play game
```

### Phase 3: Document in README
```markdown
## Optional: Audio Support
For game audio (not just terminal beeps):
    brew install sdl2 sdl2_mixer  # macOS
    sudo apt install libsdl2-mixer-dev  # Ubuntu
    cmake -B build -DENABLE_AUDIO=ON
```

---

## Bottom Line

**For a terminal Pac-Man game:**

| Solution | Effort | Quality | Dependencies | Verdict |
|----------|--------|---------|--------------|---------|
| **SDL_mixer** | Low (code exists) | High (real music) | Medium (SDL2) | ⭐ **Best** |
| **miniaudio** | Medium (convert assets) | Good (WAV playback) | Zero | Good |
| **Terminal beep** | Zero | Poor (often disabled) | Zero | Fallback |
| **PortAudio** | High (complex API) | Overkill | Medium | No |

**My vote: Add SDL2_mixer as optional feature, keep beep as default fallback.**

You get:
- ✓ Real game music when wanted (`-DENABLE_AUDIO=ON`)
- ✓ Zero deps for minimal builds (default)
- ✓ Use existing assets (.mid/.xm files)
- ✓ 5 minutes to implement (just CMake changes)

Want me to implement it?
