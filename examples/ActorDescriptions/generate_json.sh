#!/bin/bash

mkdir -p generated

FILES=*.yml
for f in $FILES
do
  # take action on each file. $f store current file name
  base=`basename $f .yml`
  echo "- $f -> $base.json"
  ./yaml_to_json.rb $f generated/$base.json
done
