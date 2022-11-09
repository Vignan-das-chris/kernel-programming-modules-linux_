#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/fpu/api.h>

#define OURMODNAME "fp_arithmetic"

MODULE_AUTHOR("Vignan das");
MODULE_DESCRIPTION("Demonstration why floating point arithmetic shouldnt be perfoemed in kernel mode");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

static double num = 22.0, den = 7.0, mypi;

static int __init fp_arth_init(void)
{
    pr_info("%s: inserted\n", OURMODNAME);

    kernel_fpu_begin();
    mypi = num / den;
    kernel_fpu_end();
#if 1
    pr_info("%s: PI = %.4f = %.4f\n", OURMODNAME, mypi, num / den);
#endif

    return 0;
}

static void __exit fp_arth_lkm_exit(void)
{
    kernel_fpu_begin();
    pr_info("%s: mypi = %f\n", OURMODNAME, mypi);
    kernel_fpu_end();

    pr_info("%s: removed\n", OURMODNAME);
}

module_init(fp_arth_init);
module_exit(fp_arth_exit);
