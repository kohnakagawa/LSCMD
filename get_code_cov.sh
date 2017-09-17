#!/bin/bash
for fname in `find . -name '*.gcda'`
do
    gcov $fname > /dev/null
done
