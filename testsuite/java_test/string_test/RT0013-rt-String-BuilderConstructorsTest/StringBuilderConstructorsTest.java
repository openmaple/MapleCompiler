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
public class StringBuilderConstructorsTest {
    private static int processResult = 99;
    public static void main(String[] argv) {
        System.out.println(run(argv, System.out));
    }
    public static int run(String[] argv, PrintStream out) {
        int result = 2; /* STATUS_Success*/

        try {
            StringBuilderConstructorsTest_1();
        } catch (Exception e) {
            processResult -= 10;
        }
        if (result == 2 && processResult == 99) {
            result = 0;
        }
        return result;
    }
    public static void StringBuilderConstructorsTest_1() {
        String str1_1 = new String("abc123abc");
        String str1_2 = new String(" @!.&%()*");
        String str1_3 = new String("qwertyuiop{}[]\\|asdfghjkl;:'\"zxcvbnm,.<>/?~`1234567890-=!@#$%^&*()_+ AS" +
                "DFGHJKLQWERTYUIOPZXCVBNM0x96");
        String str1_4 = new String("");
        String str1_5 = new String();
        String str2_1 = "abc123ABC";
        String str2_2 = " @!.&%()*";
        String str2_3 = "qwertyuiop{}[]\\|asdfghjkl;:'\"zxcvbnm,.<>/?~`1234567890-=!@#$%^&*()_+ ASDFGHJKLQWERTYUIOPZX" +
                "CVBNM0x96";
        String str2_4 = "";
        test1_1(str1_1);
        test1_1(str1_2);
        test1_1(str1_3);
        test1_1(str1_4);
        test1_1(str1_5);
        test1_1(str2_1);
        test1_1(str2_2);
        test1_1(str2_3);
        test1_1(str2_4);
        StringBuilder strBuilder1_1 = new StringBuilder("abc123abc");
        StringBuilder strBuilder1_2 = new StringBuilder(" @!.&%");
        StringBuilder strBuilder1_3 = new StringBuilder("qwertyuiop{}[]\\|asdfghjkl;:'\"zxcvbnm,.<>/?~`1234567890-=" +
                "!@#$%^&*()_+ ASDFGHJKLQWERTYUIOPZXCVBNM0x96");
        StringBuilder strBuilder1_4 = new StringBuilder("");
        StringBuilder strBuilder1_5 = new StringBuilder();
        test1_2(strBuilder1_1);
        test1_2(strBuilder1_2);
        test1_2(strBuilder1_3);
        test1_2(strBuilder1_4);
        test1_2(strBuilder1_5);
        // Test new StringBuilder()
        test1_3();
        // Test new StringBuilder(int capacity)
        test1_4();
        // Test new StringBuilder(CharSequence seq)
        test1_5();
    }
    private static void test1_1(String str) {
        System.out.println(new StringBuilder(str));
    }
    private static void test1_2(StringBuilder strBuilder) {
        System.out.println(strBuilder.toString());
    }
    private static void test1_3() {
        StringBuilder strBuilder1_6 = new StringBuilder();
        System.out.println(strBuilder1_6.capacity());
    }
    private static void test1_4() {
        int capacity = 20;
        StringBuilder strBuilder1_6 = new StringBuilder(capacity);
        System.out.println(strBuilder1_6.capacity());
        int capacityMin = 0;
        strBuilder1_6 = new StringBuilder(capacityMin);
        System.out.println(strBuilder1_6.capacity());
    }
    private static void test1_5() {
        CharSequence chs1_1 = "xyz";
        StringBuilder strBuilder1_7 = new StringBuilder(chs1_1);
        test1_2(strBuilder1_7);
    }
}
