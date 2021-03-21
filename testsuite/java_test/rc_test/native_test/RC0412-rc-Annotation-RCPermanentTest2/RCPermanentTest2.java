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


import java.util.ArrayList;
import com.huawei.ark.annotation.Permanent;
import java.lang.ref.WeakReference;
public class RCPermanentTest2 {
    static {
        System.loadLibrary("jniTestHelper");
    }
    public static native boolean isHeapObject(Object obj);
    public static native int refCount(Object obj);
    public static int checkNum = 0;
    static Object owner;
    public static Integer INT = new @Permanent Integer(100);
    public static Integer INT2 = new Integer(100);
    static String STRING = new @Permanent String("test");
    static Long LONG = new @Permanent Long(522222);
    static Long LONG2 = null;
    static boolean checkRC(Object obj) {
        int rc1 = refCount(obj);
        owner = obj;
        int rc2 = refCount(obj);
        owner = null;
        int rc3 = refCount(obj);
        if (rc1 != rc3) {
            throw new RuntimeException("rc incorrect!");
        }
        if (rc1 == rc2 && rc2 == rc3) {
            //如果相等，说明annotation生效，没有经过RC处理
            return false;
        } else {
            return true;
        }
    }
    /*
    验证public final static Integer INT = new @Permanent Integer(100);
  */


    static void method1() {
        boolean result = checkRC(INT);
        boolean heapResult = isHeapObject(INT);
        if ((result == false) && (heapResult == false)) {
            checkNum++;
        } else {
            System.out.println("error in method1");
            System.out.println("result:" + result + "    isHeap:" + heapResult);
            System.out.println(String.valueOf(result));
            System.out.println(String.valueOf(heapResult));
        }
    }
    /*
    验证public final static Integer INT2 = new Integer(100);
  */


    static void method2() {
        boolean result = checkRC(INT2);
        if (result == true && isHeapObject(INT2) == true) {
            checkNum++;
        } else {
            System.out.println("error in method2");
            System.out.println("result:" + result + "    isHeap:" + isHeapObject(INT2));
        }
    }
    /*
    验证static String STRING = new @Permanent String("test");
  */


    static void method3() {
       // static String STRING = new @Permanent String("test");
        boolean result = checkRC(STRING);
        if (result == true && isHeapObject(STRING) == true) {
            checkNum++;
        } else {
            System.out.println("error in method3");
            System.out.println("result:" + result + "    isHeap:" + isHeapObject(STRING));
        }
    }
    /*
    验证static Long LONG2 = new  Long("test4");
  */


    static void method4() {
        LONG2 = new Long(2222222);
        boolean result = checkRC(LONG2);
        if (result == true && isHeapObject(LONG2) == true) {
            checkNum++;
        } else {
            System.out.println("error in method4");
            System.out.println("result:" + result + "    isHeap:" + isHeapObject(LONG2));
        }
    }
    /*
    验证String obj = new @Permanent Long("test5");
  */


    static void method5() {
        //String obj = new @Permanent String("test5");
        Double obj = new @Permanent Double(3333.3);
        boolean result = checkRC(obj);
        if (result == false && isHeapObject(obj) == false) {
            checkNum++;
        } else {
            System.out.println("error in method5");
            System.out.println("result:" + result + "    isHeap:" + isHeapObject(obj));
        }
    }
    /*
    验证String obj = new String("test6");
  */


    static void method6() {
        Double obj = new Double(3333.3);
        boolean result = checkRC(obj);
        if (result == true && isHeapObject(obj) == true) {
            checkNum++;
        } else {
            System.out.println("error in method6");
            System.out.println("result:" + result + "    isHeap:" + isHeapObject(obj));
        }
    }
    /*
    验证obj =  new @Permanent Integer("test");
  */


    static volatile Integer obj = null;
    static void method7() {
        obj = new @Permanent Integer(123);
        boolean result = checkRC(obj);
        if (result == false && isHeapObject(obj) == false) {
            checkNum++;
        } else {
            System.out.println("error in  method7");
            System.out.println("result:" + result + "    isHeap:" + isHeapObject(obj));
        }
    }
    /*
    验证obj =  new @Permanent Integer("test");
  */


    static void method8() {
        obj = new Integer(123);
        boolean result = checkRC(obj);
        if (result == true && isHeapObject(obj) == true) {
            checkNum++;
        } else {
            System.out.println("error in method8");
            System.out.println("result:" + result + "    isHeap:" + isHeapObject(obj));
        }
    }
    /*
    验证static Long LONG = new @Permanent Long(522222);
  */


    static void method9() {
        boolean result = checkRC(LONG);
        if (result == false && isHeapObject(LONG) == false) {
            checkNum++;
        } else {
            System.out.println("error in method3");
            System.out.println("result:" + result + "    isHeap:" + isHeapObject(LONG));
        }
    }
    public static void main(String[] args) {
        method1();
        method2();
        method3();
        method4();
        method5();
        method6();
        method7();
        method8();
        method9();
        if (checkNum == 9) {
            System.out.println("ExpectResult");
        } else {
            System.out.println("error");
            System.out.println("checkNum:" + checkNum);
        }
    }
}
