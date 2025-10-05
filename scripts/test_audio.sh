#!/bin/bash
echo "🎵 Testing Glomph Maze SDL Audio"
echo "=================================="
echo ""
echo "✅ SDL2 libraries linked:"
otool -L build/glomph | grep SDL
echo ""
echo "✅ Sound files present:"
ls -1 build/sounds/*.mid | wc -l | xargs echo "   MIDI files:"
ls -1 build/sounds/*.xm | wc -l | xargs echo "   XM files:"
echo ""
echo "🎮 Starting game with audio enabled..."
echo "   Controls:"
echo "   - Arrow keys: Move"
echo "   - S: Toggle sound on/off"
echo "   - Q: Quit"
echo "   - ?: Help"
echo ""
echo "Press ENTER to start (make sure volume is up!)..."
read
cd build && ./glomph -b
