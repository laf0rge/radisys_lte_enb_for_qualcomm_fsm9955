#!/bin/sh
L2_LOG_DB="l2.logdb"
L3_LOG_DB="l3.logdb"
TARGET_DB="rlog.logdb"
CMD="cat"
CPY="cp -f"

$CMD $L3_LOG_DB > $TARGET_DB
$CMD $L2_LOG_DB >> $TARGET_DB

$CPY $TARGET_DB rsys/bin/
$CPY $TARGET_DB ../../bin/rsys/bin/

$CPY $L2_LOG_DB rsys/bin/
$CPY $L2_LOG_DB ../../bin/rsys/bin/

