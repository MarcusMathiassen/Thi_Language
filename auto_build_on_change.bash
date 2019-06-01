#!/usr/bin/env bash
fswatch -0 $1 | xargs -0 -n 1 -I {} "./thi -l $1"
