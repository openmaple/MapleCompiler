/*
 *- @TestCaseID: ClassInitConstructorNewInstance
 *- @RequirementName: Java Reflection
 *- @RequirementID: SR000B7N9H
 *- @TestCaseName:ClassInitConstructorNewInstance.java
 *- @Title/Destination: When call Constructor.newInstance, class is initialized; when call other methods of Constructor,
 *                      class is not initialized.
 *- @Condition: no
 *- @Brief:no:
 * -#step1: Class.forName("One" , false, One.class.getClassLoader()) and clazz.getConstructor to get Constructor of
 *          class One.
 * -#step2: Call methods of Class except newInstance(), class One is not initialized.
 * -#step3: call method newInstance(), class One is initialized.
 *- @Expect: 0\n
 *- @Priority: High
 *- @Remark: getAnnotatedReceiverType and getAnnotatedReturnType not support in Android
 *- @Source: ClassInitConstructorNewInstance.java
 *- @ExecuteClass: ClassInitConstructorNewInstance
 *- @ExecuteArgs:
 */

import java.lang.annotation.*;
import java.lang.reflect.Constructor;

public class ClassInitConstructorNewInstance {
    static StringBuffer result = new StringBuffer("");

    public static void main(String[] args) {
        try {
            Class clazz = Class.forName("One", false, One.class.getClassLoader());
            Constructor constructor = clazz.getConstructor(String.class);
            //check point 1: calling following methods, class not initialized
            constructor.equals(constructor);
            constructor.getAnnotation(A.class);
            constructor.getDeclaredAnnotations();
            constructor.getDeclaringClass();
            constructor.getExceptionTypes();
            constructor.getGenericExceptionTypes();
            constructor.getGenericParameterTypes();
            constructor.getModifiers();
            constructor.getName();
            constructor.getParameterAnnotations();
            constructor.getParameterCount();
            constructor.getParameterTypes();
            constructor.hashCode();
            constructor.isSynthetic();
            constructor.isVarArgs();
            constructor.toString();
            constructor.toGenericString();

            //check point 2: after call newInstance(), class initialized
            if (result.toString().compareTo("") == 0) {
                constructor.newInstance("newInstance");
            }
        } catch (Exception e) {
            System.out.println(e);
        }

        if (result.toString().compareTo("SuperOne") == 0) {
            System.out.println(0);
        } else {
            System.out.println(2);
        }
    }
}

@A
class Super {
    static {
        ClassInitConstructorNewInstance.result.append("Super");
    }
}

interface InterfaceSuper {
    String a = ClassInitConstructorNewInstance.result.append("|InterfaceSuper|").toString();
}

@A(i=1)
class One extends Super implements InterfaceSuper {
    static {
        ClassInitConstructorNewInstance.result.append("One");
    }

    String what = "lala";

    One(){}

    public One(String s){
        what = s;
    }
}

@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@interface A {
    int i() default 0;
    String a = ClassInitConstructorNewInstance.result.append("|InterfaceA|").toString();
}

class Two extends One {
    static {
        ClassInitConstructorNewInstance.result.append("Two");
    }
}
// EXEC:%maple  %f %build_option -o %n.so
// EXEC:%run %n.so %n %run_option | compare %f
// ASSERT: scan-full 0\n
