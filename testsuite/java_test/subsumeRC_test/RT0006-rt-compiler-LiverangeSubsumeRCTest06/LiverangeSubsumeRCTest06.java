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


public class LiverangeSubsumeRCTest06 {
    public static void main(String[] args) {
        LiverangeSubsumeRCTest06_Node test1 = new LiverangeSubsumeRCTest06_Node("test1");
        LiverangeSubsumeRCTest06_Node test2 = test1;
        LiverangeSubsumeRCTest06_Node test3 = test1;
        LiverangeSubsumeRCTest06_Node test4 = test1;
        LiverangeSubsumeRCTest06_B1 b1 = new LiverangeSubsumeRCTest06_B1("b1");
        test2.b1_0 = b1;
        test2.b1_0.add();
        test3.b1_0 = b1;
        test3.b1_0.add();
        test4.b1_0 = b1;
        test4.b1_0.add();
        test1.b1_0 = b1;
        test1.b1_0.add();
        int result = test1.sum + b1.sum;
        if (result == 402) {
            System.out.println("ExpectResult");
        }
    }
}
class LiverangeSubsumeRCTest06_Node {
    LiverangeSubsumeRCTest06_B1 b1_0;
    int a;
    int sum;
    String strObjectName;
    LiverangeSubsumeRCTest06_Node(String strObjectName) {
        b1_0 = null;
        a = 101;
        sum = 0;
        this.strObjectName = strObjectName;
    }
    void add() {
        sum = a + b1_0.a;
    }
}
class LiverangeSubsumeRCTest06_B1 {
    int a;
    int sum;
    String strObjectName;
    LiverangeSubsumeRCTest06_B1(String strObjectName) {
        a = 201;
        sum = 0;
        this.strObjectName = strObjectName;
//	    System.out.println("RC-Testing_Construction_B1_"+strObjectName);
    }
    void add() {
        sum = a + a;
    }
}