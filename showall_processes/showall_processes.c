#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(6, 0, 5)
#include <linux/sched/signal.h>
#endif
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/kallsyms.h>

#define OURMODNAME "showall_processes"

MODULE_AUTHOR("Vignan das ");
MODULE_DESCRIPTION("show all process by looping over the task list");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

static int show_prcs_in_tasklist(void)
{
    struct task_struct *p;
#define MAXLEN 128
    char tmp[MAXLEN];
    int numread = 0, n = 0, total = 0;
    char hdr[] = "     Name       |  TGID  |   PID  |  RUID |  EUID";

    pr_info("%s\n", &hdr[0]);
    rcu_read_lock();
    for_each_process(p)
    {
        memset(tmp, 0, 128);
        n = snprintf(tmp, 128, "%-16s|%8d|%8d|%7u|%7u\n", p->comm, p->tgid, p->pid,
                     __kuid_val(p->cred->uid), __kuid_val(p->cred->euid));
        numread += n;
        pr_info("%s", tmp);
        cond_resched();
        total++;
    }
    rcu_read_unlock();

    return total;
}

static int __init showall_processes_init(void)
{
    int total;

    pr_info("%s: inserted\n", OURMODNAME);
    total = show_prcs_in_tasklist();
    pr_info("%s: total no of processes on system: %d\n", OURMODNAME, total);

    return 0;
}

static void __exit showall_processes_exit(void)
{
    pr_info("%s: removed\n", OURMODNAME);
}

module_init(showall_processes_init);
module_exit(showall_processes_exit);
