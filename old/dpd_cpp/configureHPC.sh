#!/bin/bash

if [[ "$PATH" == *":/gpfs_partners/yingling/backup/Fuss/dpd_cpp/dpd_cpp/bin/"* ]] 
then
  echo "Path already up-to-date.";
else
  echo "You need to run: \`source ~/.cshrc\` when this is done running.";
  echo "set path=(\$path /gpfs_partners/yingling/backup/Fuss/dpd_cpp/dpd_cpp/bin/)" >> ~/.cshrc;
fi

if [[ "$LD_LIBRARY_PATH" == *":/usr/local/apps/gcc/484/lib64/"* ]]
then
  echo "LD_LIBRARY_PATH already up-to-date.";
else
  echo "You need to run: \`source ~/.cshrc\` when this is done running.";
  echo "setenv LD_LIBRARY_PATH \${LD_LIBRARY_PATH}:/usr/local/apps/gcc/484/lib64/" >> ~/.cshrc;
fi

if [[ "$LD_LIBRARY_PATH" == *":/gpfs_partners/yingling/backup/Fuss/dpd_cpp/dpd_cpp/lib/"* ]]
then
  echo "LD_LIBRARY_PATH already up-to-date.";
else
  echo "You need to run: \`source ~/.cshrc\` when this is done running.";
  echo "setenv LD_LIBRARY_PATH \${LD_LIBRARY_PATH}:/gpfs_partners/yingling/backup/Fuss/dpd_cpp/dpd_cpp/lib/" >> ~/.cshrc;
fi

if [[ "$PATH" == *":/gpfs_partners/yingling/backup/Fuss/dpd_cpp/dpd2/"* ]] 
then
  echo "Path already up-to-date.";
else
  echo "You need to run: \`source ~/.cshrc\` when this is done running.";
  echo "set path=(\$path /gpfs_partners/yingling/backup/Fuss/dpd_cpp/dpd2/)" >> ~/.cshrc;
fi

if [[ "$PATH" == *":/gpfs_partners/yingling/backup/Fuss/dpd_cpp/testdpd2/src/"* ]] 
then
  echo "Path already up-to-date.";
else
  echo "You need to run: \`source ~/.cshrc\` when this is done running.";
  echo "set path=(\$path /gpfs_partners/yingling/backup/Fuss/dpd_cpp/testdpd2/src/)" >> ~/.cshrc;
fi

if [[ "$LD_LIBRARY_PATH" == *":/gpfs_partners/yingling/backup/Fuss/dpd_cpp/dpd2/lib/"* ]]
then
  echo "LD_LIBRARY_PATH already up-to-date.";
else
  echo "You need to run: \`source ~/.cshrc\` when this is done running.";
  echo "setenv LD_LIBRARY_PATH \${LD_LIBRARY_PATH}:/gpfs_partners/yingling/backup/Fuss/dpd_cpp/dpd2/lib/" >> ~/.cshrc;
fi
