#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>

#define OURMODNAME "actualsize_slab"

MODULE_AUTHOR("Vignan das");
MODULE_DESCRIPTION("testing slab alloc with the ksize()");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

static int stepsz = 200000;
module_param(stepsz, int, 0644);
MODULE_PARM_DESC(stepsz,
                 "Amount to increase allocation by on each loop iteration (default=200000");

static int test_maxallocsz(void)
{
    size_t size2alloc = 100, actual_alloced;
    void *p;

    pr_info("kmalloc(      n) :  Actual : Wastage : Waste %%\n");
    while (1)
    {
        p = kmalloc(size2alloc, GFP_KERNEL);
        if (!p)
        {
            pr_alert("kmalloc fail, size2alloc=%zu\n", size2alloc);
            return -ENOMEM;
        }
        actual_alloced = ksize(p);
        pr_info("kmalloc(%7zu) : %7zu : %7zu : %3zu%%\n",
                size2alloc, actual_alloced, (actual_alloced - size2alloc),
                (((actual_alloced - size2alloc) * 100) / size2alloc));
        kfree(p);
        size2alloc += stepsz;
    }
    return 0;
}

static int __init actualsize_slab_init(void)
{
    pr_info("%s: inserted\n", OURMODNAME);
    return test_maxallocsz();
}

static void __exit actualsize_slab_exit(void)
{
    pr_info("%s: removed\n", OURMODNAME);
}

module_init(actualsize_slab_init);
module_exit(actualsize_slab_exit);
