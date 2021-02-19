#!/bin/bash
# Copyright (C) [2020-2021] Futurewei Technologies, Inc. All rights reverved.
#
# Licensed under the Mulan Permissive Software License v2
# You can use this software according to the terms and conditions of the MulanPSL - 2.0.
# You may obtain a copy of MulanPSL - 2.0 at:
#
#   https://opensource.org/licenses/MulanPSL-2.0
#
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
# FIT FOR A PARTICULAR PURPOSE.
# See the MulanPSL - 2.0 for more details.
#

[ -n "$MAPLE_ROOT" ] || { echo MAPLE_ROOT not set. Please source envsetup.sh.; exit 1; }

CURRDIR=`pwd`
WORKDIR=$CURRDIR/aarch64_use_whirl2mpl

mkdir -p $WORKDIR
cd $WORKDIR

echo ======================================================================== > cmd.log
echo ============= Use clangfe/whirl2mpl as C Frontend ======================= >> cmd.log
echo ======================================================================== >> cmd.log
echo cd $WORKDIR >> cmd.log

if [ $# -eq 0 ]; then
  src=printHuawei
else
  src=$1
fi

cp $CURRDIR/$src.c .

V=$(cd /usr/lib/gcc-cross/aarch64-linux-gnu/; ls | head -1)
FLAGS="-cc1 -emit-llvm -triple aarch64-linux-gnu -D__clang__ -D__BLOCKS__ -isystem /usr/aarch64-linux-gnu/include -isystem /usr/lib/gcc-cross/aarch64-linux-gnu/$V/include"
echo $MAPLE_ROOT/tools/open64_prebuilt/x86/aarch64/bin/clangfe $FLAGS $src.c >> cmd.log
$MAPLE_ROOT/tools/open64_prebuilt/x86/aarch64/bin/clangfe $FLAGS $src.c > doit.log 2>&1

echo $MAPLE_ROOT/tools/open64_prebuilt/x86/aarch64/bin/whirl2mpl -a $src.B >> cmd.log
$MAPLE_ROOT/tools/open64_prebuilt/x86/aarch64/bin/whirl2mpl -a $src.B >> doit.log 2>&1

echo $MAPLE_EXECUTE_BIN/maple --run=mplcg --option=\"-O2 -quiet\" $src.mpl >> cmd.log
$MAPLE_EXECUTE_BIN/maple --run=mplcg --option="-O2 -quiet" $src.mpl >> doit.log 2>&1

echo /usr/bin/aarch64-linux-gnu-gcc-$V -o $src.out $src.s >> cmd.log
/usr/bin/aarch64-linux-gnu-gcc-$V -o $src.out $src.s

echo qemu-aarch64 -L /usr/aarch64-linux-gnu/ $src.out >> cmd.log
qemu-aarch64 -L /usr/aarch64-linux-gnu/ $src.out > output.log

if [ $# -eq 0 ]; then
  cat cmd.log
  cat output.log
fi
