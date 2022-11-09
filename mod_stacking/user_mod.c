#define pr_fmt(fmt) "%s:%s(): " fmt, KBUILD_MODNAME, __func__

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#define MODNAME "user_mod"
#if 1
MODULE_LICENSE("Dual MIT/GPL");
#else
MODULE_LICENSE("MIT");
#endif

extern void sys_info2(void);
extern long get_skey(int);
extern int exp_int;

static int __init user_mod_init(void)
{
#define THE_ONE 0xfedface
    pr_info("insertion success\n");
    u64 sk = get_skey(THE_ONE);

    pr_debug(" called get_skey(), ret = 0x%llx = %llu\n", sk, sk);
    pr_debug("exp_int = %d\n", exp_int);
    sys_info2();

    return 0;
}

static void __exit user_mod_exit(void)
{
    pr_info("bids you adieu\n");
}

module_init(user_mod_init);
module_exit(user_mod_exit);
