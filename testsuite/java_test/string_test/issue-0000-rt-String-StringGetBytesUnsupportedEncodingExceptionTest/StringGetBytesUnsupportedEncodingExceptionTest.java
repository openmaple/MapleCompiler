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
import java.io.UnsupportedEncodingException;
public class StringGetBytesUnsupportedEncodingExceptionTest {
    private static int processResult = 99;
    public static void main(String argv[]) {
        System.out.println(run(argv, System.out));
    }
    public static int run(String argv[], PrintStream out) {
        int result = 3; /*STATUS_FAILED*/
        try {
            result = StringGetBytesUnsupportedEncodingExceptionTest_1();
        } catch (Exception e) {
            processResult -= 10;
        }
        if (result == 2 && processResult == 98) {
            result = 0;
        }
//        System.out.println("result: " + result);
//        System.out.println("processResult:" + processResult);
        return result;
    }
    public static int StringGetBytesUnsupportedEncodingExceptionTest_1() {
        int result1 = 3; /*STATUS_FAILED*/
        String str1_1 = new String("abc123");
//        String str1_2 = new String("      ");
//        String str1_3 = new String("abc123");
        String str1_4 = new String("");
        String str1_5 = new String();
        String str2_1 = "abc123";
//        String str2_2 = "      ";
//        String str2_3 = "abc123";
        String str2_4 = "";
//		String str2_5 = null;
        try {
            byte[] test1_1 = str1_1.getBytes("asc");
            processResult -= 10;
        } catch (UnsupportedEncodingException e1) {
            processResult--;
        }
        return 2;
    }
}
