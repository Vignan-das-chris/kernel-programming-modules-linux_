#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>

#define OURMODNAME "maxsize_slab"

MODULE_AUTHOR("Vignan das");
MODULE_DESCRIPTION(" test max alloc limit from k[m|z]alloc()");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

static int stepsz = 200000;
module_param(stepsz, int, 0644);
MODULE_PARM_DESC(stepsz,
                 "Amount to increase allocation by on each looping step (default=200000");

static int test_maxallocsz(void)
{
    size_t size2alloc = 0;

    while (1)
    {
        void *p = kmalloc(size2alloc, GFP_KERNEL);
        if (!p)
        {
            pr_alert("kmalloc fail, size2alloc=%zu\n", size2alloc);
            // WARN_ONCE(1, "kmalloc fail, size2alloc=%zu\n", size2alloc);
            return -ENOMEM;
        }
        pr_info("kmalloc(%7zu) = 0x%pK\n", size2alloc, p);
        kfree(p);
        size2alloc += stepsz;
    }
    return 0;
}

static int __init maxsize_slab_init(void)
{
    pr_info("%s: inserted\n", OURMODNAME);
    return test_maxallocsz();
}

static void __exit maxsize_slab_exit(void)
{
    pr_info("%s: removed\n", OURMODNAME);
}

module_init(maxsize_slab_init);
module_exit(maxsize_slab_exit);
