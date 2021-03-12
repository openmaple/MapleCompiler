/*
 * Copyright (c) [2021] Huawei Technologies Co.,Ltd.All rights reserved.
 *
 * OpenArkCompiler is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 *
 *     http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
 * FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
*/


import java.io.PrintStream;
interface CaseInterface {
    int FIELD = ClinitSeq002.getCount();
    CaseOtherClass MEMBER = new CaseOtherClass();
    default int uselessFunction(int a, int b) {
        return a + b;
    }
}
class CaseOtherClass {
    static int field;
    static {
        if (ClinitSeq002.getCount() == 2) {
            field = ClinitSeq002.getCount();
        }
    }
}
class CaseChild implements CaseInterface {
    static int field;
    static {
        if (ClinitSeq002.getCount() == 4) {
            field = ClinitSeq002.getCount();
        }
    }
}
public class ClinitSeq002 {
    static private int count = 0;
    public static void main(String[] argv) {
        System.out.println(run(argv, System.out) /*STATUS_TEMP*/);
    }
    static int getCount() {
        count += 1;
        return count;
    }
    private static int run(String[] argv, PrintStream out) {
        int res = 0;
        // 访问child field触发类初始化
        if (CaseChild.field != 5) {
            res = 2;
            out.println("Error, child not initialized");
        }
        // 访问CaseOtherClass field触发类初始化
        if (CaseOtherClass.field != 3) {
            res = 2;
            out.println("Error, other not initialized");
        }
        // 访问interface常量，触发初始化
        if (CaseInterface.FIELD != 1) {
            res = 2;
            out.println("Error, interface not initialized");
        }
        return res;
    }
}
// EXEC:%maple  %f %build_option -o %n.so
// EXEC:%run %n.so %n %run_option | compare %f
// ASSERT: scan-full 0\n
