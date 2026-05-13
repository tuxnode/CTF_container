#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

#define CAPACITY (4096 * 64)

// 1. secret进程退出后，它使用过的物理内存页会发生什么？查阅 kernel/vm.c 中的 uvmalloc()

// 2. 物理页分配器 kalloc() 使用链表管理空闲页的逻辑是什么，是如何归还空闲页的
         
// 3. secret进程退出后，如何再次获得它归还过的物理页


// 需要思考一下如何解析字符串
static int 
is_flag(char *p, int i)
{

  if(p[i]   == 'H' &&
    p[i+1] == 'C' &&
    p[i+2] == 'T' &&
    p[i+3] == 'F' &&
    p[i+4] == '{'){
    return i;
  }
  return 0;
}

int
main(int argc, char *argv[])
{
  // TODO: 在这里完成窃取逻辑
  // 提示：flag以HCTF为开头，已经为你写好了一个工具函数，可以选择性使用
 }
