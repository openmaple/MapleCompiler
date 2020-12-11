/*
 * - @TestCaseID: ConstructorGetExceptionTypes.java
 *- @RequirementName: Java Reflection
 *- @RequirementID: SR000B7N9H
 *- @TestCaseName:ConstructorGetExceptionTypes.java
 * - @Title/Destination: Constructor.getExceptionTypes() returns an array of length 0 if the executable declares no
 *                       exceptions in its throws clause.
 * - @Condition: no
 * - @Brief:no:
 *  -#step1: 定义含注解的内部类MyTargetTest02。
 *  -#step2：通过调用getConstructor(Class[])从内部类MyTargetTest02中获取对应的构造方法。
 *  -#step3：调用getExceptionTypes()获取抛出的异常类型的Class对象的数组。
 *  -#step4：确认返回的数组长度是0。
 * - @Expect: 0\n
 * - @Priority: High
 * - @Remark:
 * - @Source: ConstructorGetExceptionTypes.java
 * - @ExecuteClass: ConstructorGetExceptionTypes
 * - @ExecuteArgs:
 */

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.reflect.Constructor;

public class ConstructorGetExceptionTypes {
    @Retention(RetentionPolicy.RUNTIME)
    public @interface MyTarget {
        public String name();
        public String value();
    }

    public static void main(String[] args) {
        int result = 2;
        try {
            result = ConstructorGetExceptionTypes1();
        } catch (Exception e) {
            e.printStackTrace();
            result = 3;
        }
        System.out.println(result);
    }

    public static int ConstructorGetExceptionTypes1() {
        Constructor<MyTargetTest02> m;
        try {
            m = MyTargetTest02.class.getConstructor(new Class[] {ConstructorGetExceptionTypes.class, String.class});
            Class<?>[] Target = m.getExceptionTypes();
            if (Target.length == 0) {
                return 0;
            }
        } catch (SecurityException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        }
        return 2;
    }

    class MyTargetTest02 {
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
        public MyTargetTest02(String home) {
            this.home = home;
        }
    }
}
// EXEC:%maple  %f %build_option -o %n.so
// EXEC:%run %n.so %n %run_option | compare %f
// ASSERT: scan-full 0\n
