#!/bin/bash

tr -d '\r' <$1> $2_tmp
cp -f $2_tmp $2
rm -f $2_tmp
