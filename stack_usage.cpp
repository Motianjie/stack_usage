/*
 * @Author: MT
 * @Date: 2024-03-01 16:45:11
 * @FilePath: /stack_usage_cal/stack_usage_cal.cpp
 * @LastEditTime: 2024-03-01 20:38:33
 * @LastEditors: MT
 * @copyright: asensing.co
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <thread>

/*
 ----------------------------------- bottom            
s-----------------------------------                      
 ----------------------------------- 
 ----------------------------------- 
i----------------------------------- 
 ----------------------------------- rsp


z


e
 ----------------------------------- top thread base addr
*/
float thread_stack_usage_cal()
{
    size_t rsp_value;
    asm volatile ("movq %%rsp, %0" : "=rm" (rsp_value));
    pthread_attr_t attr;
    void *stack_addr;
    size_t stack_size;
    memset(&attr, 0, sizeof(pthread_attr_t));
    pthread_getattr_np(pthread_self(), &attr);
    pthread_attr_getstack(&attr, &stack_addr, &stack_size);
    pthread_attr_destroy(&attr);


    printf("stack top = %p \n", stack_addr);
    printf("stack bottom = %p \n", stack_addr + stack_size);
    printf("stack = %p \n",rsp_value );
    auto avail = rsp_value - (size_t)stack_addr;
    auto used = stack_size - avail;


    auto usage = (float)((float)used / (float)stack_size)*100;


    printf("thread stack info: stack_size =%p , stack top = %p , stack bottom = %p , used = %ld, avail = %ld, total = %ld  usage = %lf%%\n", 
        stack_size,
        stack_addr,
        used,
        avail,
        stack_size,
        usage);
    return usage;
}
void test()
{
    static unsigned long stack_size = 1024*1024*1;
    while (1)
    {
        unsigned char test[stack_size-1];
        unsigned char a = 1;
        thread_stack_usage_cal();
        stack_size+= 1024*1024*1;
    }  
}


int main()
 {
    unsigned char x = 0;
    std::thread thread1(test);
    thread1.join();
    return 0;
}