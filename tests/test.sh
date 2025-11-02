# scripts/test.sh - Run all tests

#!/bin/bash

set -e

echo "=== Building Satori ==="
make clean
make

echo ""
echo "=== Running Tests ==="
make test

echo ""
echo "=== Running Examples ==="
./bin/satori examples/hello.sat

echo ""
echo "✓ All checks passed"
