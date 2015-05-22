#!/bin/bash

if [[ "$PATH" == *":/gpfs_partners/yingling/backup/Fuss/dpd_cpp/dpd_cpp/bin/:"* ]] 
then
  echo "Path already up-to-date.";
else
  echo "set path=(\$path /gpfs_partners/yingling/backup/Fuss/dpd_cpp/dpd_cpp/bin/)" >> ~/.cshrc;
fi

if [[ "$LD_LIBRARY_PATH" == *":/usr/local/apps/gcc/473/lib64/:"* ]]
then
  echo "LD_LIBRARY_PATH already up-to-date.";
else
  echo "setenv LD_LIBRARY_PATH '\${LD_LIBRARY_PATH}:/usr/local/apps/gcc/473/lib64/'" >> ~/.cshrc;
fi
