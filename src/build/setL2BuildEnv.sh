#!/bin/bash
# This script shall copy the required qcom hexagon specific files to the appropriate folder
# The following files are customized for our LTE Qcom Project, hence we are overwriting
# Note: This script shall be run from app/rsys/l2/build folder
#
CODEPATH=../../../
QCOM_FILEPATH=../qu/qcomFiles
CMD="cp -f"
#Move the following files to respective folders
$CMD $QCOM_FILEPATH/gmodid.hpp $CODEPATH/targets/fsm99xxCDP_1/include/
$CMD $QCOM_FILEPATH/mainProc.cpp $CODEPATH/app/common/src/
$CMD $QCOM_FILEPATH/mainProcTarget.cpp $CODEPATH/targets/fsm99xxCDP_1/src/
$CMD $QCOM_FILEPATH/make.rules.vars $CODEPATH/make/rules/
$CMD $QCOM_FILEPATH/remoteModuleTable.cpp $CODEPATH/targets/fsm99xxCDP_1/src/
$CMD $QCOM_FILEPATH/remoteTestModuleTable.cpp $CODEPATH/targets/fsm99xxCDP_1/src/
# The following files commented is not required to be copied for FR3.0
#$CMD $QCOM_FILEPATH/testFrameworkMod.cpp $CODEPATH/app/modules/test/testFrameworkMod/
$CMD $QCOM_FILEPATH/ipcMessage.hpp $CODEPATH/infra/include/
$CMD $QCOM_FILEPATH/make.rules.bin $CODEPATH/make/rules/
$CMD $QCOM_FILEPATH/make.target.files $CODEPATH/targets/fsm99xxCDP_1/
$CMD $QCOM_FILEPATH/smdDataMod.cpp $CODEPATH/app/modules/smdDataMod/src/
$CMD $QCOM_FILEPATH/fapiApiInternal.h $CODEPATH/app/modules/test/macsimMod/
$CMD $QCOM_FILEPATH/fapiVsmApiInternal.h $CODEPATH/app/modules/test/macsimMod/
$CMD $QCOM_FILEPATH/macsimMod.cpp $CODEPATH/app/modules/test/macsimMod/
# The following files commented is not required to be copied for FR3.0
#$CMD $QCOM_FILEPATH/bamNdpPipePair.cpp $CODEPATH/platform/drivers/cryptoDrv/crypto5/src/
$CMD $QCOM_FILEPATH/intrCtrl.cpp $CODEPATH/infra/src/
# The following files commented is not required to be copied for FR3.0
#$CMD $QCOM_FILEPATH/osal.cpp $CODEPATH/osal/qurt/src/
#$CMD $QCOM_FILEPATH/shmDrv.cpp $CODEPATH/platform/drivers/shmem/src/
#$CMD $QCOM_FILEPATH/qurt_config.xml $CODEPATH/targets/fsm99xxCDP_1/
$CMD $QCOM_FILEPATH/ipcMsgBufPoolsSize.hpp $CODEPATH/targets/fsm99xxCDP_1/include/
$CMD $QCOM_FILEPATH/ipcFifoMap.hpp $CODEPATH/targets/fsm99xxCDP_1/include/
$CMD $QCOM_FILEPATH/cryptoMod.cpp $CODEPATH/platform/drivers/cryptoDrv/crypto5/src/
$CMD $QCOM_FILEPATH/ipcFifoBufPoolsSize.hpp $CODEPATH/targets/fsm99xxCDP_1/include/
$CMD $QCOM_FILEPATH/targetDefs.hpp $CODEPATH/targets/fsm99xxCDP_1/include/
$CMD $QCOM_FILEPATH/cryptoMod.hpp $CODEPATH/platform/drivers/cryptoDrv/crypto5/include/
$CMD $QCOM_FILEPATH/profile.hpp   $CODEPATH/osal/qurt/include/profile.hpp
$CMD $QCOM_FILEPATH/lte-l2.make   $CODEPATH/../LINUX/target/fsm9900_cdp/rules/lte-l2.make
