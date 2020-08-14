#!/bin/bash

echo ANDROID_PRODUCT_OUT:$ANDROID_PRODUCT_OUT

#ANDROID_PRODUCT_OUT = $1
#echo ANDROID_PRODUCT_OUT:$1
CURDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

#1.copy TEE STATIC LIB
SYSTEM_STATIC_LIB_PATH=$ANDROID_PRODUCT_OUT/obj/STATIC_LIBRARIES
GLOBALTASK_LIB_PATH=${SYSTEM_STATIC_LIB_PATH}/libtee_intermediates/libtee.a
GLOBALTASK_TUI_PATH=${SYSTEM_STATIC_LIB_PATH}/libtui_intermediates/libtui.a
GLOBALTASK_TUICORE_PATH=${SYSTEM_STATIC_LIB_PATH}/libtui-core_intermediates/libtui-core.a
GLOBALTASK_THIRD_PATH=${SYSTEM_STATIC_LIB_PATH}/libthird_intermediates/libthird.a
GLOBALTASK_THIRDTUI_PATH=${SYSTEM_STATIC_LIB_PATH}/libthirdtui_intermediates/libthirdtui.a
GLOBALTASK_O=${SYSTEM_STATIC_LIB_PATH}/global_task_o_intermediates/global_task.o

cp -rf ${GLOBALTASK_LIB_PATH} ${CURDIR}/internal_tasks/
cp -rf ${GLOBALTASK_TUI_PATH} ${CURDIR}/internal_tasks/
cp -rf ${GLOBALTASK_TUICORE_PATH} ${CURDIR}/internal_tasks/
cp -rf ${GLOBALTASK_THIRD_PATH} ${CURDIR}/internal_tasks/
cp -rf ${GLOBALTASK_THIRDTUI_PATH} ${CURDIR}/internal_tasks/
cp -rf ${GLOBALTASK_O} ${CURDIR}/internal_tasks/


#2. copy elf_main_entry.o
SYSTEM_ELF_MAIN_PATH=$ANDROID_PRODUCT_OUT/obj/EXECUTABLES/task_storage_intermediates/dotdot/elf_main_entry.o
cp -rf ${SYSTEM_ELF_MAIN_PATH} ${CURDIR}/internal_tasks/

#3. copy libtrustedcore.a
TRUSTCORE_LIB_PATH=${SYSTEM_STATIC_LIB_PATH}/libtrustedcore_intermediates/libtrustedcore.a
cp -rf ${TRUSTCORE_LIB_PATH} ${CURDIR}/trustedcore_lib/


#3. copy internal_tasks
IMG_PATH=$ANDROID_PRODUCT_OUT/obj/EXECUTABLES

#DST_PATH=${ANDROID_PRODUCT_OUT}/trustedcore.img
#CURDIR="${ cd "${ dirname "${BASH_SOURCE[0]}" }" && pwd }"

TASK1_NAME=globaltask.img
TASK1_UUID="00000000000000000000000000000000"
TASK1_PATH=${IMG_PATH}/${TASK1_NAME}_intermediates/${TASK1_NAME}

TASK2_NAME=task_teetest.elf
TASK2_UUID="03030303030303030303030303030303"

TASK3_NAME=task_echo.elf
TASK3_UUID="01010101010101010101010101010101"

TASK4_NAME=task_crypto.elf
TASK4_UUID="04040404040404040404040404040404"

TASK5_NAME=task_storage.elf
TASK5_UUID="02020202020202020202020202020202"

TASK6_NAME=task_efuse.elf
TASK6_UUID="05050505050505050505050505050505"

TASK7_NAME=task_hdcp.elf
TASK7_UUID="06060606060606060606060606060606"

TASK8_NAME=task_keymaster.elf
TASK8_UUID="07070707070707070707070707070707"

TASK9_NAME=task_secboot.elf
TASK9_UUID="08080808080808080808080808080808"

TASK10_NAME=imax_crypto.elf
TASK10_UUID="09090909090909090909090909090909"

TASK11_NAME=task_reet.elf
TASK11_UUID="0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A0A"

TASK12_NAME=task_ssa.elf
TASK12_UUID="999286b954da42359e7796e81fea1ee4"

TASK2_PATH=${IMG_PATH}/${TASK2_NAME%.*}_intermediates/${TASK2_NAME}
TASK3_PATH=${IMG_PATH}/${TASK3_NAME%.*}_intermediates/${TASK3_NAME}
TASK4_PATH=${IMG_PATH}/${TASK4_NAME%.*}_intermediates/${TASK4_NAME}
TASK5_PATH=${IMG_PATH}/${TASK5_NAME%.*}_intermediates/${TASK5_NAME}
TASK6_PATH=${IMG_PATH}/${TASK6_NAME%.*}_intermediates/${TASK6_NAME}
TASK7_PATH=${IMG_PATH}/${TASK7_NAME%.*}_intermediates/${TASK7_NAME}
TASK8_PATH=${IMG_PATH}/${TASK8_NAME%.*}_intermediates/${TASK8_NAME}
TASK9_PATH=${IMG_PATH}/${TASK9_NAME%.*}_intermediates/${TASK9_NAME}
TASK10_PATH=${IMG_PATH}/${TASK10_NAME%.*}_intermediates/${TASK10_NAME}
TASK11_PATH=${IMG_PATH}/${TASK11_NAME%.*}_intermediates/${TASK11_NAME}
TASK12_PATH=${IMG_PATH}/${TASK12_NAME%.*}_intermediates/${TASK12_NAME}

cp -rf ${TASK2_PATH} ${CURDIR}/internal_tasks/
cp -rf ${TASK3_PATH} ${CURDIR}/internal_tasks/
cp -rf ${TASK4_PATH} ${CURDIR}/internal_tasks/
cp -rf ${TASK5_PATH} ${CURDIR}/internal_tasks/
cp -rf ${TASK6_PATH} ${CURDIR}/internal_tasks/
#cp -rf ${TASK7_PATH} ${CURDIR}/internal_tasks/
#cp -rf ${TASK8_PATH} ${CURDIR}/internal_tasks/
#cp -rf ${TASK9_PATH} ${CURDIR}/internal_tasks/
#cp -rf ${TASK10_PATH} ${CURDIR}/internal_tasks/
cp -rf ${TASK11_PATH} ${CURDIR}/internal_tasks/
cp -rf ${TASK12_PATH} ${CURDIR}/internal_tasks/

