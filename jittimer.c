/*
    file Name:      jittimer.c
    Author:         zengweitotty
    version:        V1.0
    Data:           2013/03/28
    Email:          zengweitotty@gmail.com
    Description     test for the timer
*/
#include <linux/init.h>	//using for module_init and some related
#include <linux/kernel.h>	//using for printk and some related
#include <linux/module.h>	
#include <linux/hardirq.h>	//using for in_interrupt
#include <linux/timer.h>	//using for struct timer_list
#include <linux/proc_fs.h>	//using for create_proc_read_entry
#include <linux/moduleparam.h>	//using for module_param
#include <linux/slab.h>	//using for kmalloc
#include <linux/errno.h>	//using for Error Number
#include <linux/jiffies.h>	//using for jiffies
#include <linux/wait.h>	//using for wait
#include <linux/sched.h>	//using for schedule

#define JIT_LOOP 10

static int jit_loop = 0;
module_param(jit_loop,int,S_IRUGO);

struct jit_data{
	struct timer_list timer;
	char* buf;
	int len;
	unsigned long prevjiffies;
	wait_queue_head_t wait;
};

struct jit_data *jitData;

static void jittimer_timer_fn(unsigned long data){	//call back function
	struct jit_data* jitDatatemp = (struct jit_data *)data;
	jitDatatemp->buf  += sprintf(jitDatatemp->buf, "%9li\t%3li\t%i\t%6i\t%i\t%s\n",jiffies,jiffies - jitData->prevjiffies,in_interrupt()?1:0,current->pid,smp_processor_id(),current->comm);
	if(--jit_loop){
		printk(KERN_INFO "The seq is %d",jit_loop);
		jitDatatemp->prevjiffies = jiffies;
		jitDatatemp->timer.expires = jiffies + HZ;
		jitDatatemp->timer.function = jittimer_timer_fn;
		jitDatatemp->timer.data = (unsigned long)jitDatatemp;
		add_timer(&jitDatatemp->timer);
	}else{
		wake_up_interruptible(&jitData->wait);		
	}
}

static int jittimer_fn(char *buf,char** start,off_t offset,int len,int *eof,void *data){
	jitData->buf = buf;
	jitData->prevjiffies = jiffies;
	jitData->buf += sprintf(jitData->buf,"timer\tdelta\tinirq\tpid\tcpu\tcommand\n");
	jitData->timer.expires = jiffies + HZ;
	jitData->timer.function = jittimer_timer_fn;
	jitData->timer.data = (unsigned long)jitData;
	add_timer(&jitData->timer);
	wait_event_interruptible(jitData->wait,!jit_loop);
//	if (signal_pending(current))
//		return -ERESTARTSYS;
	printk(KERN_INFO "[jittimer/jittimer_fn] Success to wake up jitData timer\n");
	del_timer_sync(&jitData->timer);
	*start = buf;
	jitData->len = jitData->buf - buf;
	return jitData->len;
}

static int __init jittimer_init(void){
	jitData = kmalloc(sizeof(struct jit_data),GFP_KERNEL);
	if(!jitData){
		printk(KERN_ERR "[jittimer/jittimer_init] Can not kmalloc memory\n");		
		return -ENOMEM;
	}
	memset(jitData,0,sizeof(struct jit_data));
	init_timer(&jitData->timer);
	init_waitqueue_head(&jitData->wait);	//init wait queue
	create_proc_read_entry("jittimer",0,NULL,jittimer_fn,NULL);
	printk(KERN_INFO "[jittimer/jittimer_init] Success to create jittimer proc\n");
	return 0;
}
static void __exit jittimer_exit(void){
	kfree(jitData);
	remove_proc_entry("jittimer",NULL);
	printk(KERN_INFO "[jittimer/jittimer_exit] Success to remove jittimer proc\n");
}

module_init(jittimer_init);
module_exit(jittimer_exit);

MODULE_AUTHOR("zengweitotty");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("timer for test");
