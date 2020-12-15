/*
 *- @TestCaseID:Alloc_61_64x8B
 *- @TestCaseName:MyselfClassName
 *- @RequirementName:[运行时需求]支持自动内存管理
 *- @Title:ROS Allocator is in charge of applying and releasing objects.This testcase is mainly for testing objects from 61*8B to 64*8B(max)
 *- @Condition: no
 * -#c1
 *- @Brief:functionTest
 * -#step1
 *- @Expect:ExpectResult\n
 *- @Priority: High
 *- @Source: Alloc_61_64x8B.java
 *- @ExecuteClass: Alloc_61_64x8B
 *- @ExecuteArgs:
 *- @Remark:
 */
import java.util.ArrayList;

public class Alloc_61_64x8B {
    private  final static int PAGE_SIZE=4*1024;
    private final static int OBJ_HEADSIZE = 8;
    private final static int MAX_61_8B=61*8;
    private final static int MAX_62_8B=62*8;
    private final static int MAX_63_8B=63*8;
    private final static int MAX_64_8B=64*8;
    private static ArrayList<byte[]> store;

    private static int alloc_test(int slot_type){
        store=new ArrayList<byte[]>();
        byte[] temp;
        int i;
        if(slot_type==24){
            i=1;}
        else if(slot_type==1024){
            i=64*8+1-OBJ_HEADSIZE;
        }else{
            i=slot_type-2*8+1;
        }

        for(;i<=slot_type-OBJ_HEADSIZE;i++)
        {
            for(int j=0;j<(PAGE_SIZE*7/(i+OBJ_HEADSIZE)+10);j++)
            {
                temp = new byte[i];
                store.add(temp);
            }
        }
        int check_size=store.size();
        store=new ArrayList<byte[]>();
        return check_size;
    }
    public static void main(String[] args) {
        store = new ArrayList<byte[]>();
        int countSize61 = alloc_test(MAX_61_8B);
        int countSize62 = alloc_test(MAX_62_8B);
        int countSize63 = alloc_test(MAX_63_8B);
        int countSize64 = alloc_test(MAX_64_8B);
        //System.out.println(countSize61);
        //System.out.println(countSize62);
        //System.out.println(countSize63);
        //System.out.println(countSize64);

        if (countSize61 == 549 && countSize62 == 542 && countSize63 == 535 && countSize64 == 528 )
            System.out.println("ExpectResult");
        else
            System.out.println("Error");
    }
}
// EXEC:%maple  %f %build_option -o %n.so
// EXEC:%run %n.so %n %run_option | compare %f
// ASSERT: scan-full ExpectResult\n
