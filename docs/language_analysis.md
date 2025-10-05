# Should Glomph Maze be Rewritten in Python?

## Current Situation
- 11,399 lines of C code
- Deep ncurses integration
- 1997-2009 codebase with modern updates
- Terminal-based real-time game
- 4,558 lines still in myman.c monolith

## The Honest Truth

### Why C Made Sense Originally (1997)
✅ Performance - real-time game loop
✅ Direct ncurses access
✅ Portable across Unix systems
✅ Small binary size
✅ No runtime dependencies

### Why Python Might Make Sense Now (2025)

**Pros:**
1. **Maintainability**
   - Clean separation of concerns naturally
   - Modern patterns (classes, modules)
   - Much less code (probably 2,000-3,000 lines total)

2. **Development Speed**
   - No compile step
   - Easy testing
   - Rich ecosystem (pytest, type hints)

3. **Terminal Libraries**
   - `curses` module (Python wrapper)
   - `blessed` (modern alternative)
   - `rich` (beautiful terminal UIs)
   - `textual` (TUI framework)

4. **Code Quality**
   ```python
   # Python version would look like:
   class Terminal:
       def __init__(self):
           self.screen = curses.initscr()
       
       def move(self, y, x):
           self.screen.move(y, x)
       
       def putch(self, ch, color):
           self.screen.addch(ch, curses.color_pair(color))
   
   class GameState:
       def __init__(self):
           self.score = 0
           self.lives = 3
           self.player = Player()
           self.ghosts = [Ghost() for _ in range(4)]
   
   # vs C's global variables and 4,558 line files
   ```

5. **Modern Patterns**
   - Easy dependency injection
   - Natural separation of concerns
   - Testable without terminal
   - Type hints for safety

**Cons:**
1. **Performance**
   - Python is slower (but for a Pac-Man game? doesn't matter)
   - Real-time game loop might have latency
   - Frame timing less precise

2. **Distribution**
   - Needs Python runtime
   - Not a single binary
   - Dependency management (pip, venv)

3. **Legacy**
   - Throws away 20+ years of C code
   - Loses some heritage
   - Would need to port all mazes, sprites, etc.

4. **Terminal Libraries**
   - Python curses is quirky
   - Not as low-level control as C

## The REAL Question

**Are we refactoring or rewriting?**

If we're doing a "proper" refactor with:
- Terminal abstraction layer
- Clean separation of concerns
- Testable modules
- Proper architecture

...we're essentially rewriting anyway. So why not do it in a language
that ENCOURAGES good architecture instead of fighting against it?

## Hybrid Approach

**Option 1: Rewrite Core in Python, Keep Assets**
```
glomph-maze/
├── game/           # Python game engine
│   ├── terminal.py
│   ├── engine.py
│   ├── entities.py
│   └── renderer.py
├── assets/         # Keep existing mazes, sprites
│   ├── mazes/
│   └── sprites/
└── glomph.py       # Entry point
```
Time: ~2-3 weeks for clean rewrite
Result: Maintainable, modern codebase

**Option 2: Keep C, Do Minimal Fixes**
- Accept myman.c as is
- Extract pager only
- Add section comments
- Focus on new features
Time: ~3-4 days
Result: Still have 4,000 line file, but working

**Option 3: C++ Refactor**
- Use classes for proper encapsulation
- Keep performance
- Better than C for architecture
- But still a rewrite
Time: ~3-4 weeks
Result: Better C code, still complex

**Option 4: Rust Rewrite**
- Modern, safe, fast
- Great for this use case
- Learning curve
- Terminal libraries available
Time: ~4-6 weeks (with learning)
Result: Modern, safe, maintainable

## Performance Reality Check

Let's be honest: **Pac-Man doesn't need C performance**

- Game runs at 60 FPS max
- Simple collision detection
- 4 ghosts with basic AI
- Static maze

Python can EASILY handle this. The original ran on 1980s hardware
at 3.5 MHz. Modern Python on a laptop? No problem.

## My Recommendation

### Short Answer: **Python Rewrite**

**Why:**
1. You said it feels like a complete rewrite - it IS
2. If we're rewriting anyway, use the right tool
3. Python naturally enforces good architecture
4. 2-3 weeks vs 6+ months fighting C
5. Result will be maintainable long-term

**What to Keep:**
- All maze files (they're just text)
- All sprite files (they're just text)
- Game rules and logic (port to Python)
- SDL audio integration (pygame has this)

**What to Gain:**
- ~3,000 lines instead of 11,000
- Clean architecture by default
- Easy testing
- Easy to add features
- Modern development experience

### Alternative: **Accept the C Monolith**

If Python rewrite feels wrong:
- Accept myman.c as "the engine"
- Add section markers for navigation
- Extract pager only (easy win)
- Stop fighting the architecture
- Focus on features/documentation

## The Uncomfortable Truth

The current C codebase is fighting us because:
1. It wasn't designed for modularity
2. C doesn't encourage modularity
3. We're trying to impose modern patterns on 1997 code
4. Every extraction reveals more coupling

We can spend months untangling it, or 2-3 weeks rewriting
in a language designed for what we're trying to achieve.

## Your Decision

Three paths:
1. **Python rewrite** - Fresh start, modern, maintainable
2. **Keep fighting C** - Months of refactoring, uncertain outcome
3. **Accept C as-is** - Document, minor fixes, move on

What matters most to you?
- Performance? (C wins, but Python is fine)
- Maintainability? (Python wins easily)
- Learning? (Rust wins for modern systems programming)
- Speed to done? (Python wins)
- Preserving legacy? (Keep C)

