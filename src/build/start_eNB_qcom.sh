#!/bin/sh
#logger "RADISYS:Inside start_eNB.sh"
FILENAME="start_eNB.sh"
BANKPATH=$2
CONFIGPATH=$1
#logger "RADISYS:$FILENAME:Setting memory parameters for kernel socket buffers"
sysctl -w net.core.rmem_max=16777216
sysctl -w net.core.rmem_default=16777216
sysctl -w net.core.wmem_max=16777216
sysctl -w net.core.wmem_default=16777216










#logger "RADISYS:$FILENAME:                                                             "
#logger "RADISYS:$FILENAME:                                                             "
#logger "RADISYS:$FILENAME:                                                             "
#logger "RADISYS:$FILENAME:                           ((       ))                       "
#logger "RADISYS:$FILENAME:                            ((     ))                        "
#logger "RADISYS:$FILENAME:                             ((   ))                         "
#logger "RADISYS:$FILENAME:   / ::::  /::::::         ||   *   :::::: ::    :: ::::::   "
#logger "RADISYS:$FILENAME:  : /      ||   ||     ....||  | |  ||      ::  ::  ||       "
#logger "RADISYS:$FILENAME:  | |      ||   ||    ||   ||  | |  ::::::    ||    ::::::   "
#logger "RADISYS:$FILENAME:  | |      ||   ::    ||   ||  | |      ||    ||        ||   "
#logger "RADISYS:$FILENAME:  | |      ::::::,,,  ||:::::  | |  ::::::    ||    ::::::   "

#logger "RADISYS:$FILENAME:                                                             "
#logger "RADISYS:$FILENAME:                                                             "
#logger "RADISYS:$FILENAME:                                                             "
#logger "RADISYS:$FILENAME:                                                             "
#logger "RADISYS:$FILENAME:                                                             "
#logger "RADISYS:$FILENAME:   TotaleNodeB Solution for QCOM 9955 PP 65101               "
#logger "RADISYS:$FILENAME:                                                             "
#logger "RADISYS:$FILENAME:                                                             "
#logger "RADISYS:$FILENAME:                                                             "

#sleep 10 
cd $BANKPATH/rsys/bin
ulimit -c unlimited
#logger "RADISYS:$FILENAME:Removing the L3 core Dump and L2 Halt event"
rm -rf /mnt/flash/halt_event/*
rm -rf /mnt/flash/coredumps/*
sync
#logger "RADISYS:$FILENAME:Launching enodeB with OAM"
#gdbserver :1234 ./enodeb
./enodeb
