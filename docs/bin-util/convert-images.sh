#!/bin/bash

# Usage: convert-images.sh file1.png file2.jpg ...
# produces: file1-converted.png file2-converted.jpg ...

for i in $*; do
    extension=${i##*.}
    path_without_extension=${i%.*}
    convert $i -resize 50% -bordercolor black -border 2 ${path_without_extension}-converted.${extension}
done
