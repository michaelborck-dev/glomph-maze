# Python CLI Executable Options

## The Problem
- User wants: `brew install glomph-maze` → runs without Python visible
- Python needs: Runtime interpreter + dependencies

## Solutions (Best to Worst)

### 1. PyInstaller (BEST for CLI apps)
**What it does:** Bundles Python + your code + dependencies into ONE executable

```bash
# Install
pip install pyinstaller

# Create executable
pyinstaller --onefile glomph.py

# Result: dist/glomph (single binary!)
# User runs: ./glomph
# No Python visible, no dependencies needed
```

**Pros:**
✅ Single binary output
✅ Works on macOS, Linux, Windows
✅ No Python installation needed
✅ Can bundle assets (mazes, sprites)
✅ Popular, well-tested

**Cons:**
❌ ~10-15MB binary (includes Python runtime)
❌ Slightly slower startup (~1 second)
❌ Still just bundled Python (not native)

**Size:** ~10-15MB (vs 200KB for C binary)

---

### 2. Nuitka (Best Performance)
**What it does:** Compiles Python → C → native binary

```bash
pip install nuitka
nuitka --onefile --follow-imports glomph.py

# Result: glomph.bin (compiled binary)
```

**Pros:**
✅ TRUE native binary
✅ Faster than PyInstaller
✅ Better performance than interpreted Python
✅ Smaller binary (~5-8MB)

**Cons:**
❌ Compilation takes longer
❌ Less mature than PyInstaller
❌ Can have compatibility issues

---

### 3. Shiv (Zipapp - Python's Official Way)
**What it does:** Creates executable .pyz file

```bash
pip install shiv
shiv -c glomph -o glomph.pyz .

# User runs: ./glomph.pyz
# But needs Python installed
```

**Pros:**
✅ Official Python packaging
✅ Fast, simple
✅ Small size

**Cons:**
❌ Requires Python on system
❌ Not truly standalone

---

### 4. Docker/Snap/Flatpak
**What it does:** Bundle everything in container

**Pros:**
✅ Complete isolation
✅ All dependencies included

**Cons:**
❌ Requires Docker/Snap/Flatpak
❌ Overkill for CLI game
❌ Large size (100MB+)

---

## Homebrew Distribution

### For Python Version:
```ruby
# glomph-maze.rb (Homebrew formula)
class GlomphMaze < Formula
  desc "Terminal Pac-Man game"
  homepage "https://github.com/you/glomph-maze"
  url "https://github.com/you/glomph-maze/releases/download/v1.0/glomph-1.0.tar.gz"
  
  depends_on "python@3.11"
  
  def install
    virtualenv_install_with_resources
  end
end
```

**User experience:**
```bash
brew install glomph-maze
glomph  # Just works! Homebrew handles Python
```

### For PyInstaller Binary:
```ruby
class GlomphMaze < Formula
  desc "Terminal Pac-Man game"
  homepage "https://github.com/you/glomph-maze"
  url "https://github.com/you/glomph-maze/releases/download/v1.0/glomph-macos-arm64.tar.gz"
  
  def install
    bin.install "glomph"
    pkgshare.install "assets"
  end
end
```

**User experience:**
```bash
brew install glomph-maze
glomph  # Pure binary, no Python visible
```

---

## Real-World Examples

### CLI tools using PyInstaller:
- **youtube-dl** (before yt-dlp) - Distributed as single binary
- **aws-cli** v1 - Bundled with PyInstaller
- **httpie** - Available as binary via PyInstaller

### CLI tools shipping Python:
- **poetry** - Homebrew installs with Python
- **pipx** - Python-based, users don't care
- **black** - Formatter, PyInstaller binary available

---

## Size Comparison

**C binary (current):**
- Size: ~200KB
- Startup: instant
- Distribution: single file

**Python + PyInstaller:**
- Size: ~10-15MB
- Startup: ~1 second
- Distribution: single file

**Python + Homebrew:**
- Size: ~2MB Python code + Python runtime (shared)
- Startup: ~0.5 seconds
- Distribution: Homebrew manages it

---

## My Recommendation

### Option 1: PyInstaller Binary (BEST)
**Ship a single 15MB binary**

Pros:
- True "install and run" experience
- No Python visible to user
- Works like C binary
- Easy distribution

Cons:
- 15MB vs 200KB (but who cares in 2025?)
- 1 second startup vs instant (acceptable)

### Option 2: Homebrew with Python (EASIER)
**Let Homebrew handle Python**

Pros:
- Standard Python packaging
- Smaller download (2MB)
- Easier to maintain
- Most Homebrew users have Python anyway

Cons:
- Depends on Python (but Homebrew handles it)
- Not "standalone" (but transparent to user)

---

## The Verdict

**YES, you can create standalone Python executables!**

**Best approach:**
1. Develop in Python with clean architecture
2. Use PyInstaller to create binary: `glomph-macos-arm64`, `glomph-linux-x64`
3. Distribute via Homebrew (macOS), AUR (Arch), .deb (Ubuntu)
4. Users run `brew install glomph-maze` → get single binary

**User experience is IDENTICAL to C binary:**
```bash
$ brew install glomph-maze
$ glomph --help
$ glomph
# Game runs, no Python visible
```

**The 15MB size?** No one cares in 2025. Slack is 150MB. VS Code is 200MB.
A terminal Pac-Man game at 15MB is totally acceptable.

---

## Should You Still Care About C?

**Keep C if:**
- You want 200KB binary (but 15MB is fine)
- You want instant startup (but 1s is fine)
- You're learning systems programming
- You value the heritage

**Choose Python if:**
- You want maintainability
- You want clean architecture
- You want to finish in weeks not months
- You want to easily add features

The binary size and startup time arguments for C are mostly **obsolete** for a game like this.

