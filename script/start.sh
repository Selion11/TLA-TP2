#! /bin/bash

set -e
ROOT="$(dirname "$0")"

echo "Relocating script to '$ROOT'..."
cd "$ROOT/.."

cat "$1" | ./bin/Compiler

dot -Tpng ast_graph.dot -o ast_graph.png