/*
 *- @TestCaseID:maple/runtime/rc/function/RC_Thread01/Cycle_am_00430.java
 *- @TestCaseName:MyselfClassName
 *- @RequirementID:SR-10620538
 *- @RequirementName:[运行时需求]支持自动内存管理
 *- @Title/Destination:Change Cycle_a_00430 in RC测试-Cycle-01 to Multi thread testcase.
 *- @Condition: no
 * -#c1
 *- @Brief:functionTest
 * -#step1
 *- @Expect:ExpectResult\nExpectResult\n
 *- @Priority: High
 *- @Source: Cycle_am_00430.java
 *- @ExecuteClass: Cycle_am_00430
 *- @ExecuteArgs:
 *- @Remark:
 */

class ThreadRc_Cycle_am_00430 extends Thread {
    private boolean checkout;

    public void run() {
        Cycle_a_00430_A1 a1_main = new Cycle_a_00430_A1("a1_main");
        a1_main.a2_0 = new Cycle_a_00430_A2("a2_0");
        a1_main.a2_0.a3_0 = new Cycle_a_00430_A3("a3_0");
        a1_main.a2_0.a4_0 = new Cycle_a_00430_A4("a4_0");
        a1_main.a2_0.a1_0 = a1_main;
        a1_main.a2_0.a3_0.a1_0 = a1_main;
        a1_main.a2_0.a4_0.a2_0 = a1_main.a2_0;
        a1_main.a2_0.a4_0.a1_0 = a1_main;


        a1_main.add();
        a1_main.a2_0.add();
        a1_main.a2_0.a3_0.add();
        a1_main.a2_0.a4_0.add();


        int result = a1_main.sum + a1_main.a2_0.sum + a1_main.a2_0.a3_0.sum + a1_main.a2_0.a4_0.sum;
        //System.out.println("RC-Testing_Result="+result);

        if (result == 1124)
            checkout = true;
        //System.out.println(checkout);
    }

    public boolean check() {
        return checkout;
    }

    class Cycle_a_00430_A1 {
        Cycle_a_00430_A2 a2_0;
        int a;
        int sum;
        String strObjectName;

        Cycle_a_00430_A1(String strObjectName) {
            a2_0 = null;
            a = 101;
            sum = 0;
            this.strObjectName = strObjectName;
//	    System.out.println("RC-Testing_Construction_A1_"+strObjectName);
        }

        void add() {
            sum = a + a2_0.a;
        }
    }

    class Cycle_a_00430_A2 {
        Cycle_a_00430_A1 a1_0;
        Cycle_a_00430_A3 a3_0;
        Cycle_a_00430_A4 a4_0;
        int a;
        int sum;
        String strObjectName;

        Cycle_a_00430_A2(String strObjectName) {
            a1_0 = null;
            a3_0 = null;
            a4_0 = null;
            a = 102;
            sum = 0;
            this.strObjectName = strObjectName;
//	    System.out.println("RC-Testing_Construction_A2_"+strObjectName);
        }

        void add() {
            sum = a + a1_0.a + a3_0.a + a4_0.a;
        }
    }

    class Cycle_a_00430_A3 {
        Cycle_a_00430_A1 a1_0;
        int a;
        int sum;
        String strObjectName;

        Cycle_a_00430_A3(String strObjectName) {
            a1_0 = null;
            a = 103;
            sum = 0;
            this.strObjectName = strObjectName;
//	    System.out.println("RC-Testing_Construction_A3_"+strObjectName);
        }

        void add() {
            sum = a + a1_0.a;
        }
    }

    class Cycle_a_00430_A4 {
        Cycle_a_00430_A1 a1_0;
        Cycle_a_00430_A2 a2_0;
        int a;
        int sum;
        String strObjectName;

        Cycle_a_00430_A4(String strObjectName) {
            a2_0 = null;
            a = 104;
            sum = 0;
            this.strObjectName = strObjectName;
//	    System.out.println("RC-Testing_Construction_A4_"+strObjectName);
        }

        void add() {
            sum = a + a1_0.a + a2_0.a;
        }
    }
}


public class Cycle_am_00430 {
    public static void main(String[] args) {
        rc_testcase_main_wrapper();
	Runtime.getRuntime().gc();
	rc_testcase_main_wrapper();
        
    }

    private static void rc_testcase_main_wrapper() {
        ThreadRc_Cycle_am_00430 A1_Cycle_am_00430 = new ThreadRc_Cycle_am_00430();
        ThreadRc_Cycle_am_00430 A2_Cycle_am_00430 = new ThreadRc_Cycle_am_00430();
        ThreadRc_Cycle_am_00430 A3_Cycle_am_00430 = new ThreadRc_Cycle_am_00430();
        ThreadRc_Cycle_am_00430 A4_Cycle_am_00430 = new ThreadRc_Cycle_am_00430();
        ThreadRc_Cycle_am_00430 A5_Cycle_am_00430 = new ThreadRc_Cycle_am_00430();
        ThreadRc_Cycle_am_00430 A6_Cycle_am_00430 = new ThreadRc_Cycle_am_00430();

        A1_Cycle_am_00430.start();
        A2_Cycle_am_00430.start();
        A3_Cycle_am_00430.start();
        A4_Cycle_am_00430.start();
        A5_Cycle_am_00430.start();
        A6_Cycle_am_00430.start();

        try {
            A1_Cycle_am_00430.join();
            A2_Cycle_am_00430.join();
            A3_Cycle_am_00430.join();
            A4_Cycle_am_00430.join();
            A5_Cycle_am_00430.join();
            A6_Cycle_am_00430.join();

        } catch (InterruptedException e) {
        }
        if (A1_Cycle_am_00430.check() && A2_Cycle_am_00430.check() && A3_Cycle_am_00430.check() && A4_Cycle_am_00430.check() && A5_Cycle_am_00430.check() && A6_Cycle_am_00430.check())
            System.out.println("ExpectResult");
    }
}
// EXEC:%maple  %f %build_option -o %n.so
// EXEC:%run %n.so %n %run_option | compare %f
// ASSERT: scan-full ExpectResult\nExpectResult\n
