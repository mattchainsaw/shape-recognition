#!/bin/bash

if [ -z $1 ]
then
  echo "Usage: bash $0 {data point file}"
  exit
fi

PWD=`pwd`
DATA=$PWD/$1
MED_DATA=$DATA.medial
EXE=medialAxis

# if c binary isn't made, make it
if [ ! -x $EXE ]
then
  make
fi

# if $MED_DATA isn't made, make it
if [ ! -f $MED_DATA ]
then
  ./$EXE $DATA > $MED_DATA
fi

# show the medial axis
# change /usr/.../matlab to matlab if not trial, or wherever you got it
/usr/local/MATLAB/R2015a/bin/matlab -nodesktop -nosplash -q -r "visualize('$DATA','$MED_DATA');pause;exit"
