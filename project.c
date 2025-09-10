#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/delay.h>
#include <linux/sched/signal.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel Module for Process Creation and Memory Allocation");
MODULE_AUTHOR("SGG");

struct task_struct *child_task[3];

int process_function(void *data) {
    unsigned long memory;
    pid_t pid = current->pid;

    printk(KERN_INFO "[Child %d] Running. Memory allocated at ", pid);

    memory = (unsigned long)kmalloc(1024, GFP_KERNEL);
    if (!memory) {
        printk(KERN_ERR "[Child %d] Memory allocation failed\n", pid);
        return -ENOMEM;
    }

    printk(KERN_CONT "0x%lx\n", memory);

    msleep(2000);
    
    kfree((void *)memory);
    printk(KERN_INFO "[Child %d] Exiting. Memory freed.\n", pid);

    return 0;
}

static int __init kernel_module_init(void) {
    int i;
    pid_t parent_pid = current->pid;

    printk(KERN_INFO "[Parent %d] Module loaded. Creating child processes...\n", parent_pid);

    for (i = 0; i < 3; i++) {
        child_task[i] = kthread_run(process_function, NULL, "child_thread_%d", i);
        if (IS_ERR(child_task[i])) {
            printk(KERN_ERR "[Parent] Failed to create child %d\n", i);
            child_task[i] = NULL;
        } else {
            printk(KERN_INFO "[Parent] Created child %d with PID %d\n", i, child_task[i]->pid);
        }
    }

    return 0;
}

static void __exit kernel_module_exit(void) {
    int i;
    printk(KERN_INFO "[Parent] Module unloading...\n");

    for (i = 0; i < 3; i++) {
        if (child_task[i] && !IS_ERR(child_task[i])) {
            printk(KERN_INFO "[Parent] Stopping child %d (PID %d)\n", i, child_task[i]->pid);
            kthread_stop(child_task[i]);
        }
    }

    printk(KERN_INFO "[Parent] Module unloaded.\n");
}

module_init(kernel_module_init);
module_exit(kernel_module_exit);
