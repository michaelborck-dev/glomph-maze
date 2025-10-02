# Help System Fix

## Root Cause
`gamehelp()` sets `reinit_requested = 1`, which causes:
1. Game loop breaks (line 7200)  
2. do-while reinits and clears `reinit_requested = 0` (line 7209)
3. `pager()` is called (line 7210)
4. **BUG**: Something in the first pager loop iteration sets `reinit_requested = 1` again
5. Pager exits immediately with `reinit=1`
6. do-while condition `} while (reinit_requested)` (line 7285) is TRUE
7. **INFINITE LOOP** - Steps 2-6 repeat forever

## The Fix
Need to prevent `reinit_requested` from being set during help pager display.
Most likely cause: The pager is immediately breaking because `pager_remaining` 
is not being advanced, or input handling is broken.

## Next Steps
Check why the pager loop only runs once before exiting with reinit=1.
