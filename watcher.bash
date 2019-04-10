#!/usr/bin/env bash
fswatch -0 ast.json | xargs -0 -n 1 -I {} sh -c "clear && fx ast.json ."
