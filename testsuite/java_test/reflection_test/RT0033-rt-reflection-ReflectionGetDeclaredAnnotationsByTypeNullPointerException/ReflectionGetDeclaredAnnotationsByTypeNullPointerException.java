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
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Inherited
@interface Dddd2 {
    int i() default 0;
    String t() default "";
}
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Inherited
@interface Dddd2_a {
    int i_a() default 2;
    String t_a() default "";
}
@Dddd2(i = 333, t = "test1")
class GetDeclaredAnnotationsByType2 {
    public int i;
    public String t;
}
@Dddd2_a(i_a = 666, t_a = "right1")
class GetDeclaredAnnotationsByType2_a extends GetDeclaredAnnotationsByType2 {
    public int i_a;
    public String t_a;
}
class GetDeclaredAnnotationsByType2_b extends GetDeclaredAnnotationsByType2_a {
}
public class ReflectionGetDeclaredAnnotationsByTypeNullPointerException {
    public static void main(String[] args) {
        int result = 0; /* STATUS_Success*/

        try {
            Class zqp1 = Class.forName("GetDeclaredAnnotationsByType2_b");
            Annotation[] j = zqp1.getDeclaredAnnotationsByType(null);
            result = 0;
        } catch (ClassNotFoundException e1) {
            System.err.println(e1);
            result = -1;
        } catch (NullPointerException e2) {
            result = 0;
        }
        System.out.println(result);
    }
}