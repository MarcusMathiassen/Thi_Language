#!/usr/bin/env bash
fswatch -0 ./src ./tests | xargs -0 -n 1 -I {} ./build_dev.bash
