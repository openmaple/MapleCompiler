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


import java.lang.Runtime;
class RC_Thread_12_1 extends Thread {
    public void run() {
        RC_Thread_12 rcth01 = new RC_Thread_12();
        try {
            rcth01.setA1null();
        } catch (NullPointerException e) {
        }
    }
}
class RC_Thread_12_2 extends Thread {
    public void run() {
        RC_Thread_12 rcth01 = new RC_Thread_12();
        try {
            rcth01.setA4null();
        } catch (NullPointerException e) {
        }
    }
}
class RC_Thread_12_3 extends Thread {
    public void run() {
        RC_Thread_12 rcth01 = new RC_Thread_12();
        try {
            rcth01.setA5null();
        } catch (NullPointerException e) {
        }
    }
}
public class RC_Thread_12 {
    private static RC_Thread_12_A1 a1_main = null;
    private static RC_Thread_12_A4 a4_main = null;
    private static RC_Thread_12_A5 a5_main = null;
    RC_Thread_12() {
        try {
            a1_main = new RC_Thread_12_A1();
            a1_main.a2_0 = new RC_Thread_12_A2();
            a1_main.a2_0.a3_0 = new RC_Thread_12_A3();
            a4_main = new RC_Thread_12_A4();
            a5_main = new RC_Thread_12_A5();
            a4_main.a1_0 = a1_main;
            a5_main.a1_0 = a1_main;
            a1_main.a2_0.a3_0.a1_0 = a1_main;
        } catch (NullPointerException e) {
        }
    }
    public static void main(String[] args) {
        cycle_pattern_wrapper();
        rc_testcase_main_wrapper();
		Runtime.getRuntime().gc();
        rc_testcase_main_wrapper();
		Runtime.getRuntime().gc();
        rc_testcase_main_wrapper();
        Runtime.getRuntime().gc();
        rc_testcase_main_wrapper();
        System.out.println("ExpectResult");
    }
    private static void cycle_pattern_wrapper(){
        a1_main = new RC_Thread_12_A1();
        a1_main.a2_0 = new RC_Thread_12_A2();
        a1_main.a2_0.a3_0 = new RC_Thread_12_A3();
        a4_main = new RC_Thread_12_A4();
        a5_main = new RC_Thread_12_A5();
        a4_main.a1_0 = a1_main;
        a5_main.a1_0 = a1_main;
        a1_main.a2_0.a3_0.a1_0 = a1_main;
        a1_main=null;
        a4_main=null;
        a5_main=null;
        Runtime.getRuntime().gc();
    }
    private static void rc_testcase_main_wrapper() {
        RC_Thread_12_1 t1 = new RC_Thread_12_1();
        RC_Thread_12_2 t2 = new RC_Thread_12_2();
        RC_Thread_12_3 t3 = new RC_Thread_12_3();
        t1.start();
        t2.start();
        t3.start();
        try {
            t1.join();
            t2.join();
            t3.join();
        } catch (InterruptedException e) {
        }
    }
    public void setA1null() {
        a1_main = null;
    }
    public void setA4null() {
        a4_main = null;
    }
    public void setA5null() {
        a5_main = null;
    }
    static class RC_Thread_12_A1 {
        RC_Thread_12_A2 a2_0;
        int a;
        int sum;
        RC_Thread_12_A1() {
            a2_0 = null;
            a = 1;
            sum = 0;
        }
        void add() {
            sum = a + a2_0.a;
        }
    }
    static class RC_Thread_12_A2 {
        RC_Thread_12_A3 a3_0;
        int a;
        int sum;
        RC_Thread_12_A2() {
            a3_0 = null;
            a = 2;
            sum = 0;
        }
        void add() {
            sum = a + a3_0.a;
        }
    }
    static class RC_Thread_12_A3 {
        RC_Thread_12_A1 a1_0;
        int a;
        int sum;
        RC_Thread_12_A3() {
            a1_0 = null;
            a = 3;
            sum = 0;
        }
        void add() {
            sum = a + a1_0.a;
        }
    }
    static class RC_Thread_12_A4 {
        RC_Thread_12_A1 a1_0;
        int a;
        int sum;
        RC_Thread_12_A4() {
            a1_0 = null;
            a = 4;
            sum = 0;
        }
        void add() {
            sum = a + a1_0.a;
        }
    }
    static class RC_Thread_12_A5 {
        RC_Thread_12_A1 a1_0;
        int a;
        int sum;
        RC_Thread_12_A5() {
            a1_0 = null;
            a = 5;
            sum = 0;
        }
        void add() {
            sum = a + a1_0.a;
        }
    }
}
