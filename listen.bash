#!/usr/bin/env bash
fswatch -0 $1 | xargs -0 -n 1 -I {} sh -c "./thi -l $1"
