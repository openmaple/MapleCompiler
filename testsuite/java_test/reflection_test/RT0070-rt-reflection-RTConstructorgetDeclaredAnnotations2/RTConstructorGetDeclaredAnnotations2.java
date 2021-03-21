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
import java.lang.reflect.Constructor;
@Target(ElementType.CONSTRUCTOR)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Inherited
@interface IF2 {
    int i() default 0;
    String t() default "";
}
@Target(ElementType.CONSTRUCTOR)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Inherited
@interface IF2_a {
    int i_a() default 2;
    String t_a() default "";
}
class ConstructorGetDeclaredAnnotations2 {
    public ConstructorGetDeclaredAnnotations2() {
    }
    @IF2(i = 333, t = "test1")
    public ConstructorGetDeclaredAnnotations2(String name) {
    }
    @IF2(i = 333, t = "test1")
    ConstructorGetDeclaredAnnotations2(int number) {
    }
}
class ConstructorGetDeclaredAnnotations2_a extends ConstructorGetDeclaredAnnotations2 {
    public ConstructorGetDeclaredAnnotations2_a(String name) {
    }
    ConstructorGetDeclaredAnnotations2_a(int number) {
    }
}
public class RTConstructorGetDeclaredAnnotations2 {
    public static void main(String[] args) {
        try {
            Class cls = Class.forName("ConstructorGetDeclaredAnnotations2_a");
            Constructor instance1 = cls.getConstructor(String.class);
            Constructor instance2 = cls.getDeclaredConstructor(int.class);
            if (instance1.getDeclaredAnnotations().length == 0 && instance2.getDeclaredAnnotations().length == 0) {
                System.out.println(0);
                return;
            }
            System.out.println(1);
            return;
        } catch (ClassNotFoundException e) {
            System.out.println(2);
            return;
        } catch (NoSuchMethodException e1) {
            System.out.println(3);
            return;
        } catch (NullPointerException e2) {
            System.out.println(4);
            return;
        }
    }
}