#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/preempt.h>
#include <linux/cred.h>
#include <linux/uidgid.h>

#define OURMODNAME "running_processes"

MODULE_AUTHOR("Vignan das");
MODULE_DESCRIPTION("Display the members of currently running process");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

static inline void show_ctx(char *nm)
{
    unsigned int uid = from_kuid(&init_user_ns, current_uid());
    unsigned int euid = from_kuid(&init_user_ns, current_euid());

    pr_info("%s:%s():%d ", nm, __func__, __LINE__);
    if (likely(in_task()))
    {
        pr_info("%s: in process context ::\n"
                " PID         : %6d\n"
                " TGID        : %6d\n"
                " UID         : %6u\n"
                " EUID        : %6u (%s root)\n"
                " name        : %s\n"
                " current (ptr to our process context's task_struct) :\n"
                "               0x%pK (0x%px)\n"
                " stack start : 0x%pK (0x%px)\n",
                nm,
                task_pid_nr(current), task_tgid_nr(current),
                uid, euid,
                (euid == 0 ? "have" : "don't have"),
                current->comm,
                current, current,
                current->stack, current->stack);
    }
    else
        pr_alert("%s: in interrupt context wrongly attempt\n", nm);
}

static int __init running_process_init(void)
{
    pr_info("%s: inserted\n", OURMODNAME);
    pr_info(" sizeof(struct task_struct)=%zd\n", sizeof(struct task_struct));
    show_ctx(OURMODNAME);
    return 0;
}

static void __exit running_process_exit(void)
{
    show_ctx(OURMODNAME);
    pr_info("%s: removed\n", OURMODNAME);
}

module_init(running_process_init);
module_exit(running_process_exit);
