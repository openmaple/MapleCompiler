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


import java.lang.Thread;
public class CharacterSubsetExObjectnotifyIllegalMonitorStateException {
    static int res = 99;
    MySubset2 mf2 = new MySubset2("some subset");
    public static void main(String argv[]) {
        System.out.println(new CharacterSubsetExObjectnotifyIllegalMonitorStateException().run());
    }
    /**
     * main test fun
     *
     * @return status code
    */

    public int run() {
        int result = 2; /*STATUS_FAILED*/
        try {
            result = characterSubsetExObjectnotifyIllegalMonitorStateException1();
        } catch (Exception e) {
            CharacterSubsetExObjectnotifyIllegalMonitorStateException.res = CharacterSubsetExObjectnotifyIllegalMonitorStateException.res - 20;
        }
        Thread t1 = new Thread(new CharacterSubsetExObjectnotifyIllegalMonitorStateException11());
        t1.setUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() {
            @Override
            public void uncaughtException(Thread t, Throwable e) {
                System.out.println(t.getName() + " : " + e.getMessage());
            }
        });
        t1.start();
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        if (result == 4 && CharacterSubsetExObjectnotifyIllegalMonitorStateException.res == 58) {
            result = 0;
        }
        return result;
    }
    private int characterSubsetExObjectnotifyIllegalMonitorStateException1() {
        int result1 = 4; /*STATUS_FAILED*/
        // IllegalMonitorStateException - if the current thread is not the owner of the object's monitor.
        //
        // final void notify()
        try {
            mf2.notify();
            CharacterSubsetExObjectnotifyIllegalMonitorStateException.res = CharacterSubsetExObjectnotifyIllegalMonitorStateException.res - 10;
        } catch (IllegalMonitorStateException e2) {
            CharacterSubsetExObjectnotifyIllegalMonitorStateException.res = CharacterSubsetExObjectnotifyIllegalMonitorStateException.res - 1;
        }
        return result1;
    }
    private class CharacterSubsetExObjectnotifyIllegalMonitorStateException11 implements Runnable {
        // final void notify()
        /**
         * Thread run fun
        */

        public void run() {
            synchronized (mf2) {
                try {
                    mf2.notify();
                    CharacterSubsetExObjectnotifyIllegalMonitorStateException.res = CharacterSubsetExObjectnotifyIllegalMonitorStateException.res - 40;
                } catch (IllegalMonitorStateException e2) {
                    CharacterSubsetExObjectnotifyIllegalMonitorStateException.res = CharacterSubsetExObjectnotifyIllegalMonitorStateException.res - 30;
                }
            }
        }
    }
}
class MySubset2 extends Character.Subset {
    MySubset2(String name) {
        super(name);
    }
}