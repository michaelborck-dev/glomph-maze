#!/bin/bash

# Script to generate Doxygen and cflow documentation
# Run from project root directory

set -e  # Exit on error

echo "=== Generating Documentation ==="
echo ""

# Create output directories if they don't exist
mkdir -p docs/cflow

# Generate Doxygen documentation
echo "Running Doxygen..."
cd docs
doxygen Doxyfile
cd ..
echo "✓ Doxygen documentation generated in docs/generated/"
echo ""

# Generate cflow call graphs
echo "Running cflow..."

# Tree format (hierarchical call graph)
cflow --tree src/*.c > docs/cflow/callgraph-tree.txt 2>&1
echo "✓ Tree call graph: docs/cflow/callgraph-tree.txt"

# Detailed format with line numbers
cflow -l src/*.c > docs/cflow/callgraph-detailed.txt 2>&1
echo "✓ Detailed call graph: docs/cflow/callgraph-detailed.txt"

# Reverse call graph (what calls each function)
cflow -r src/*.c > docs/cflow/callgraph-reverse.txt 2>&1
echo "✓ Reverse call graph: docs/cflow/callgraph-reverse.txt"

echo ""
echo "=== Documentation Generation Complete ==="
echo ""
echo "View documentation:"
echo "  HTML: open docs/generated/html/index.html"
echo "  XML:  docs/generated/xml/"
echo "  cflow: docs/cflow/"
