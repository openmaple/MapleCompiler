/*
 * - @TestCaseID: FieldToGenericString
 *- @RequirementName: Java Reflection
 *- @RequirementID: SR000B7N9H
 *- @TestCaseName:FieldToGenericString.java
 * - @Title/Destination: Field.toGenericString() returns a string describing this Field, including its generic type.
 * - @Condition: no
 * - @Brief:no:
 *  -#step1: 定义含注解的内部类MyTargetTest10。
 *  -#step2：通过调用getField()从内部类MyTargetTest10中获取home。
 *  -#step3：调用toGenericString()获取描述此Field的字符串。
 *  -#step4：确认返回的描述正确。
 * - @Expect: 0\n
 * - @Priority: High
 * - @Remark:
 * - @Source: FieldToGenericString.java
 * - @ExecuteClass: FieldToGenericString
 * - @ExecuteArgs:
 */

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.reflect.Field;

public class FieldToGenericString {
    @Retention(RetentionPolicy.RUNTIME)
    public @interface MyTarget {
        public String name();
        public String value();
    }

    public static void main(String[] args) {
        int result = 2;
        try {
            result = FieldToGenericString1();
        } catch (Exception e) {
            e.printStackTrace();
            result = 3;
        }
        System.out.println(result);
    }

    public static int FieldToGenericString1() {
        Field m;
        try {
            m = MyTargetTest10.class.getField("home");
            String str = m.toGenericString();
            if ("public java.lang.String FieldToGenericString$MyTargetTest10.home".equals(str)) {
                return 0;
            }
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (NoSuchFieldException e) {
            e.printStackTrace();
        }
        return 2;
    }

    class MyTargetTest10 {
        @MyTarget(name = "newName", value = "newValue")
        public String home;

        @MyTarget(name = "name", value = "value")
        public void MyTargetTest_1() {
            System.out.println("This is Example:hello world");
        }

        public void newMethod(@MyTarget(name = "name1", value = "value1") String home) {
            System.out.println("my home at:" + home);
        }

        @MyTarget(name = "cons", value = "constructor")
        public MyTargetTest10(String home) {
            this.home = home;
        }
    }
}
// EXEC:%maple  %f %build_option -o %n.so
// EXEC:%run %n.so %n %run_option | compare %f
// ASSERT: scan-full 0\n
