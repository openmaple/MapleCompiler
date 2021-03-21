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


import java.lang.annotation.*;
import java.lang.reflect.Field;
public class ClassInitFieldSetCharStatic {
    static StringBuffer result = new StringBuffer("");
    public static void main(String[] args) {
        try {
            Class clazz = Class.forName("One", false, One.class.getClassLoader());
            Field f = clazz.getField("hiChar");
            if (result.toString().compareTo("") == 0) {
                f.setChar(null, (char)1);
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
        ClassInitFieldSetCharStatic.result.append("Super");
    }
}
interface InterfaceSuper {
    String a = ClassInitFieldSetCharStatic.result.append("|InterfaceSuper|").toString();
}
@A(i=1)
class One extends Super implements InterfaceSuper {
    static {
        ClassInitFieldSetCharStatic.result.append("One");
    }
    public static char hiChar = (char)45;
}
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@interface A {
    int i() default 0;
    String a = ClassInitFieldSetCharStatic.result.append("|InterfaceA|").toString();
}
class Two extends One {
    static {
        ClassInitFieldSetCharStatic.result.append("Two");
    }
}