#define pr_fmt(fmt) "%s:%s(): " fmt, KBUILD_MODNAME, __func__

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#define OURMODNAME "mod_template"

MODULE_AUTHOR("Mr.vignan das");
MODULE_DESCRIPTION("module template");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

static int __init mod_template_init(void)
{
    pr_info("insertion done\n");
    return 0;
}

static void __exit mod_template_exit(void)
{
    pr_info("removed\n");
}

module_init(mod_template_init);
module_exit(mod_template_exit);
