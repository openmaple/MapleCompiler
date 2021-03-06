/*
 *- @TestCaseID:maple/runtime/rc/annotation/RCUnownedLocalTest
 *- @TestCaseName:MyselfClassName
 *- @RequirementName:[运行时需求]支持自动内存管理
 *- @Title/Destination:方法中添加@UnownedLocal,没有环泄露
 *- @Condition:no
 * -#c1:
 *- @Brief:functionTest
 * -#step1:
 * 方法中添加@UnownedLocal,没有环泄露
 *- @Expect:ExpectResult\n
 *- @Priority: High
 *- @Source: RCUnownedLocalTest.java
 *- @ExecuteClass: RCUnownedLocalTest
 *- @ExecuteArgs:
 *- @Remark:
 *- @Author:liuweiqing l00481345
 */

import com.huawei.ark.annotation.UnownedLocal;

import java.util.LinkedList;
import java.util.List;

public class RCUnownedLocalTest {
    static Integer a = new Integer(1);
    static Object[] arr = new Object[]{1, 2, 3};

    @UnownedLocal
    static int method(Integer a, Object[] arr) {
        int check = 0;
        Integer c = a + a;
        if (c == 2) {
            check++;
        } else {
            check--;
        }
        for (Object array : arr) {
            //System.out.println(array);
            check++;
        }
        return check;
    }

    public static void main(String[] args) {
        int result = method(a, arr);
        if (result == 4) {
            System.out.println("ExpectResult");
        }
    }

}

// EXEC:%maple  %f %build_option -o %n.so
// EXEC:%run %n.so %n %run_option | compare %f
// ASSERT: scan-full ExpectResult\n
