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
public class CharacterSubsetExObjectwaitIllegalArgumentException {
    static int res = 99;
    MySubset4 csb1 = new MySubset4("some subset");
    public static void main(String argv[]) {
        System.out.println(new CharacterSubsetExObjectwaitIllegalArgumentException().run());
    }
    private class CharacterSubsetExObjectwaitIllegalArgumentException21 implements Runnable {
        // IllegalArgumentException - if the value of timeout is negative.
        // final void wait(long millis)
        long millis = -1;
        /**
         * Thread run fun
        */

        public void run() {
            synchronized (csb1) {
                csb1.notifyAll();
                try {
                    csb1.wait(millis);
                    CharacterSubsetExObjectwaitIllegalArgumentException.res = CharacterSubsetExObjectwaitIllegalArgumentException.res - 15;
                } catch (InterruptedException e1) {
                    CharacterSubsetExObjectwaitIllegalArgumentException.res = CharacterSubsetExObjectwaitIllegalArgumentException.res - 20;
                } catch (IllegalMonitorStateException e2) {
                    CharacterSubsetExObjectwaitIllegalArgumentException.res = CharacterSubsetExObjectwaitIllegalArgumentException.res - 10;
                } catch (IllegalArgumentException e3) {
                    CharacterSubsetExObjectwaitIllegalArgumentException.res = CharacterSubsetExObjectwaitIllegalArgumentException.res - 5;
                }
            }
        }
    }
    private class CharacterSubsetExObjectwaitIllegalArgumentException31 implements Runnable {
        // IllegalArgumentException - if the value of timeout is negative or the value of nanos is not in the range 0-999999.
        // final void wait(long millis, int nanos)
        long millis = -2;
        int nanos = 10;
        /**
         * Thread run fun
        */

        public void run() {
            synchronized (csb1) {
                csb1.notifyAll();
                try {
                    csb1.wait(millis, nanos);
                    CharacterSubsetExObjectwaitIllegalArgumentException.res = CharacterSubsetExObjectwaitIllegalArgumentException.res - 15;
                } catch (InterruptedException e1) {
                    CharacterSubsetExObjectwaitIllegalArgumentException.res = CharacterSubsetExObjectwaitIllegalArgumentException.res - 20;
                } catch (IllegalMonitorStateException e2) {
                    CharacterSubsetExObjectwaitIllegalArgumentException.res = CharacterSubsetExObjectwaitIllegalArgumentException.res - 10;
                } catch (IllegalArgumentException e3) {
                    CharacterSubsetExObjectwaitIllegalArgumentException.res = CharacterSubsetExObjectwaitIllegalArgumentException.res - 5;
                }
            }
        }
    }
    /**
     * sleep fun
     * @param sleepNum wait time
    */

    public void sleep(int sleepNum) {
        try {
            Thread.sleep(sleepNum);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
    /**
     * main test fun
     *
     * @return status code
    */

    public int run() {
        int result = 2; /*STATUS_FAILED*/
        // final void wait(long millis)
        Thread t3 = new Thread(new CharacterSubsetExObjectwaitIllegalArgumentException21());
        // final void wait(long millis, int nanos)
        Thread t5 = new Thread(new CharacterSubsetExObjectwaitIllegalArgumentException31());
        t3.setUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() {
            @Override
            public void uncaughtException(Thread t, Throwable e) {
                System.out.println(t.getName() + " : " + e.getMessage());
            }
        });
        t5.setUncaughtExceptionHandler(new Thread.UncaughtExceptionHandler() {
            @Override
            public void uncaughtException(Thread t, Throwable e) {
                System.out.println(t.getName() + " : " + e.getMessage());
            }
        });
        t3.start();
        t5.start();
        sleep(1000);
        if (result == 2 && CharacterSubsetExObjectwaitIllegalArgumentException.res == 89) {
            result = 0;
        }
        return result;
    }
}
class MySubset4 extends Character.Subset {
    MySubset4(String name) {
        super(name);
    }
}