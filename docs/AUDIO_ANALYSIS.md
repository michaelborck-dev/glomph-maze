# Audio System Analysis

## Current Implementation

The game has **3 audio backends** selected at compile-time:

### 1. SDL_Mixer (if `USE_SDL_MIXER` is defined)
- Plays `.xm` or `.mid` files from `assets/sounds/`
- High-quality music playback
- Requires SDL and SDL_mixer libraries

### 2. Terminal Beep (if `USE_BEEP` is defined, default)
- Simple `beep()` function from curses
- No external dependencies
- Very basic audio feedback

### 3. Silent (fallback)
- No audio at all
- Just clears the sfx flags

## Current Status

**We are using: Terminal Beep (option 2)**

Evidence:
- `USE_SDL_MIXER` is **NOT** defined in CMakeLists.txt
- `USE_BEEP` defaults to `1` in myman.c line ~3485
- No SDL dependencies in CMakeLists.txt
- Only curses is linked

## Why Terminal Beep?

**Advantages:**
✓ Zero external dependencies (already have curses)
✓ Works everywhere ncurses works
✓ Small code footprint
✓ No library version conflicts

**Disadvantages:**
✗ Very basic audio (just beeps)
✗ No music or complex sound effects
✗ Annoying beep sound on some terminals

## Modern Alternatives

### Option 1: Add SDL_Mixer Support
**Pros:**
- High-quality audio
- Can play the existing .mid/.xm files in assets/sounds/
- Cross-platform

**Cons:**
- Adds SDL dependency (large)
- Complicates build system
- May conflict with ncurses in some terminals

### Option 2: Use miniaudio (Modern, lightweight)
**Pros:**
- Single-header library (no dependencies)
- Cross-platform
- Can play WAV, MP3, FLAC
- Very active development (2024)

**Cons:**
- Would need to convert .mid/.xm files to WAV/MP3
- Larger than terminal beep

### Option 3: Use PortAudio
**Pros:**
- Lightweight, cross-platform
- Industry standard
- Good performance

**Cons:**
- Still requires external library
- Overkill for simple game audio

### Option 4: Keep Terminal Beep
**Pros:**
- Zero work required
- Already works
- Minimal dependencies
- Retro/authentic feel

**Cons:**
- Basic audio only

## Recommendation

**Keep Terminal Beep for now** because:

1. **Simplicity First** - We're modernizing the codebase, not adding features
2. **Zero Dependencies** - Matches our C17 + ncurses-only goal
3. **Cross-Platform** - Works everywhere without library versioning issues
4. **Optional Future** - Can add SDL_Mixer later as opt-in feature

If we want better audio in the future:
- Add CMake option: `ENABLE_SDL_AUDIO` (default OFF)
- Keep beep as fallback
- Document as optional enhancement

## Current Code State

The audio code is already well-abstracted:
```c
#if USE_SDL_MIXER
    // SDL implementation
#elif USE_BEEP
    // Beep implementation (CURRENT)
#else
    // Silent implementation
#endif
```

No changes needed unless we want to add SDL support.
