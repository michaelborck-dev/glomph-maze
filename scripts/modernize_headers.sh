#!/bin/bash
# Replace guess.h platform detection with modern POSIX assumptions

set -e

echo "Modernizing headers for POSIX systems (Linux/macOS/WSL)..."

# Step 1: Remove guess.h includes
for file in src/*.c; do
    echo "Processing $file..."
    # Remove the guess.h include block
    sed -i '' '/\/\* feature guessing \*\//,/#include "guess.h"/d' "$file"
    sed -i '' '/#ifndef MYMAN_GUESS_H_INCLUDED/,/#endif.*MYMAN_GUESS_H_INCLUDED/d' "$file"
done

# Step 2: Remove Windows/DOS platform code
echo "Removing Windows/DOS platform code..."
unifdef -UWIN32 -U__MSDOS__ -UMAIN_NO_ENVP -m src/*.c

# Step 3: Remove VMS code
echo "Removing VMS code..."  
unifdef -UVMS -m src/*.c

# Step 4: Assume all standard POSIX headers exist
echo "Assuming standard POSIX headers..."
unifdef -DHAVE_FCNTL_H=1 \
        -DHAVE_LOCALE_H=1 \
        -DHAVE_SYS_IOCTL_H=1 \
        -DHAVE_SYS_STAT_H=1 \
        -DHAVE_SYS_TIME_H=1 \
        -DHAVE_UNISTD_H=1 \
        -DHAVE_WCHAR_H=1 \
        -DHAVE_LANGINFO_H=1 \
        -DHAVE_STDINT_H=1 \
        -DHAVE_PUTENV=1 \
        -DHAVE_SETENV=1 \
        -m src/*.c

# Step 5: Remove Windows-only headers
echo "Removing Windows-only headers..."
unifdef -UHAVE_IO_H -UHAVE_UNIXIO_H -m src/*.c

echo "Done!"
echo "Now test with: cmake --build build"
