#!/bin/bash

# Generate CHANGELOG.md from git history

cat > CHANGELOG.md << 'EOF'
# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## Unreleased

EOF

git log --pretty=format:"- %s (%h)" --no-merges >> CHANGELOG.md

echo "Changelog generated."