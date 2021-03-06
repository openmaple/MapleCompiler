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
public class CharacterSubsetExObjectnotifyAllIllegalMonitorStateException {
    static int res = 99;
    MySubset1 mf2 = new MySubset1("some subset");
    public static void main(String argv[]) {
        System.out.println(new CharacterSubsetExObjectnotifyAllIllegalMonitorStateException().run());
    }
    /**
     * main test fun
     *
     * @return status code
    */

    public int run() {
        int result = 2; /*STATUS_FAILED*/
        try {
            result = characterSubsetExObjectnotifyAllIllegalMonitorStateException1();
        } catch (Exception e) {
            CharacterSubsetExObjectnotifyAllIllegalMonitorStateException.res = CharacterSubsetExObjectnotifyAllIllegalMonitorStateException.res - 20;
        }
        Thread t1 = new Thread(new CharacterSubsetExObjectnotifyAllIllegalMonitorStateException11());
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
        if (result == 4 && CharacterSubsetExObjectnotifyAllIllegalMonitorStateException.res == 58) {
            result = 0;
        }
        return result;
    }
    private int characterSubsetExObjectnotifyAllIllegalMonitorStateException1() {
        int result1 = 4; /*STATUS_FAILED*/
        // IllegalMonitorStateException - if the current thread is not the owner of the object's monitor.
        //  final void notifyAll()
        try {
            mf2.notifyAll();
            CharacterSubsetExObjectnotifyAllIllegalMonitorStateException.res = CharacterSubsetExObjectnotifyAllIllegalMonitorStateException.res - 10;
        } catch (IllegalMonitorStateException e2) {
            CharacterSubsetExObjectnotifyAllIllegalMonitorStateException.res = CharacterSubsetExObjectnotifyAllIllegalMonitorStateException.res - 1;
        }
        return result1;
    }
    private class CharacterSubsetExObjectnotifyAllIllegalMonitorStateException11 implements Runnable {
        //  final void notifyAll()
        /**
         * Thread run fun
        */

        public void run() {
            synchronized (mf2) {
                try {
                    mf2.notifyAll();
                    CharacterSubsetExObjectnotifyAllIllegalMonitorStateException.res = CharacterSubsetExObjectnotifyAllIllegalMonitorStateException.res - 40;
                } catch (IllegalMonitorStateException e2) {
                    CharacterSubsetExObjectnotifyAllIllegalMonitorStateException.res = CharacterSubsetExObjectnotifyAllIllegalMonitorStateException.res - 30;
                }
            }
        }
    }
}
class MySubset1 extends Character.Subset {
    MySubset1(String name) {
        super(name);
    }
}