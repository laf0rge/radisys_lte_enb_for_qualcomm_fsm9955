#!/bin/sh -e

while [ 1 == 1 ]
do
   syncM=`ps | grep syncmgr | grep -v grep`
   if [[ $? == 0 ]];
   then
      echo "syncmgr is up "
      break
   else
      echo "Waiting for syncmgr up"
   fi
   sleep 5
done 

killall -9 syncmgr
sysctl -w net.core.rmem_max=16777216
sysctl -w net.core.rmem_default=16777216
sysctl -w net.core.wmem_max=16777216
sysctl -w net.core.rmem_max=16777216
/opt/qcom/bin/syncmgr  --primary-source=FREE_RUNNING --ttr1=0 --ftr2=50 &

while [ 1 == 1 ]
do 
   if [ -f /tmp/sclte_config_done ]
   then
      echo "SCLTE IND received"
      break
   else
      echo "Waiting for for SCLTE IND"
   fi
   sleep 5
done
echo "Starting Enodeb"
 ./enodeb


