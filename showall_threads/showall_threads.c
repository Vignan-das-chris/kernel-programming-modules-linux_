#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(6, 0, 5)
#include <linux/sched/signal.h>
#endif

#define OURMODNAME "showall_threads"

MODULE_AUTHOR("Vignan das");
MODULE_DESCRIPTION("to display all the threads looping over the tasklist");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

static inline void disp_idle_thread(void)
{
    struct task_struct *t = &init_task;

    /* We know that the swapper is a kernel thread */
    pr_info("%8d %8d   0x%px  0x%px [%16s]\n",
            t->pid, t->pid, t, t->stack, t->comm);
}

static int showthrds(void)
{
    struct task_struct *g = NULL, *t = NULL; /* 'g' : process ptr; 't': thread ptr */
    int nr_thrds = 1, total = 1;             /* total init to 1 for the idle thread */
#define BUFMAX 256
#define TMPMAX 128
    char buf[BUFMAX], tmp[TMPMAX];
    const char hdr[] =
        "------------------------------------------------------------------------------------------\n"
        "    TGID     PID         current           stack-start         Thread Name     MT? # thrds\n"
        "------------------------------------------------------------------------------------------\n";

    pr_info("%s", hdr);
    disp_idle_thread();
    rcu_read_lock();
    do_each_thread(g, t)
    { /* 'g' : process ptr; 't': thread ptr */
        task_lock(t);

        snprintf(buf, BUFMAX - 1, "%8d %8d ", g->tgid, t->pid);
        snprintf(tmp, TMPMAX - 1, "  0x%px", t);
        snprintf(buf, BUFMAX - 1, "%s%s  0x%px", buf, tmp, t->stack);

        if (!g->mm)
        {
            snprintf(tmp, TMPMAX - 1, " [%16s]", t->comm);
        }
        else
        {
            snprintf(tmp, TMPMAX - 1, "  %16s ", t->comm);
        }
        snprintf(buf, BUFMAX - 1, "%s%s", buf, tmp);

        nr_thrds = get_nr_threads(g);
        if (g->mm && (g->tgid == t->pid) && (nr_thrds > 1))
        {
            snprintf(tmp, TMPMAX - 1, " %3d", nr_thrds);
            snprintf(buf, BUFMAX - 1, "%s%s", buf, tmp);
        }

        snprintf(buf, BUFMAX - 1, "%s\n", buf);
        pr_info("%s", buf);

        total++;
        memset(buf, 0, sizeof(buf));
        memset(tmp, 0, sizeof(tmp));
        task_unlock(t);
    }
    while_each_thread(g, t);
    rcu_read_unlock();

    return total;
}

static int __init showall_threads_init(void)
{
    int total;

    pr_info("%s: inserted\n", OURMODNAME);
    total = showthrds();
    pr_info("%s: total # of threads on the system: %d\n",
            OURMODNAME, total);

    return 0;
}

static void __exit showall_threads_exit(void)
{
    pr_info("%s: removed\n", OURMODNAME);
}

module_init(showall_threads_init);
module_exit(showall_threads_exit);
