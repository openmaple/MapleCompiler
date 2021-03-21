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


import java.util.ArrayList;
class Alloc_Threadm_50x8B_01 extends Thread {
    private  final static int PAGE_SIZE=4*1024;
    private final static int OBJ_HEADSIZE = 8;
    private final static int MAX_50_8B=50*8;
    private static boolean checkout=false;
    public void run() {
        ArrayList<byte[]> store=new ArrayList<byte[]>();
        byte[] temp;
        for(int i=320-OBJ_HEADSIZE;i<=MAX_50_8B-OBJ_HEADSIZE;i=i+5)
        {
            for(int j=0;j<(PAGE_SIZE*96/(i+OBJ_HEADSIZE)+10);j++)
            {
                temp = new byte[i];
                store.add(temp);
            }
        }
        int check_size=store.size();
        //System.out.println(check_size);
        if(check_size == 18816)
            checkout=true;
    }
    public boolean check() {
        return checkout;
    }
}
public class Alloc_Threadm_50x8B {
    public static void main(String[] args){
        Alloc_Threadm_50x8B_01 test1=new Alloc_Threadm_50x8B_01();
        test1.start();
        Alloc_Threadm_50x8B_01 test2=new Alloc_Threadm_50x8B_01();
        test2.start();
        Alloc_Threadm_50x8B_01 test3=new Alloc_Threadm_50x8B_01();
        test3.start();
        Alloc_Threadm_50x8B_01 test4=new Alloc_Threadm_50x8B_01();
        test4.start();
        Alloc_Threadm_50x8B_01 test5=new Alloc_Threadm_50x8B_01();
        test5.start();
        Alloc_Threadm_50x8B_01 test6=new Alloc_Threadm_50x8B_01();
        test6.start();
        try{
            test1.join();
            test2.join();
            test3.join();
            test4.join();
            test5.join();
            test6.join();
        }catch (InterruptedException e){}
        if(test1.check()==true&&test2.check()==true && test3.check()==true&&test4.check()==true && test5.check()==true&&test6.check()==true)
            System.out.println("ExpectResult");
        else
            System.out.println("Error");
    }
}
