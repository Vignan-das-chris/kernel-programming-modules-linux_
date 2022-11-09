#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#define OUR_MODNAME "mod_params"
MODULE_AUTHOR("Vignan Das");
MODULE_DESCRIPTION("Demonstration of module parameters");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

static int mp_debug_level;
module_param(mp_debug_level, int, 0660);
MODULE_PARM_DESC(mp_debug_level,
                 "Debug level [0-2]; 0 => no debug messages, 2 => high verbosity");

static char *mp_strparam = "My string param";
module_param(mp_strparam, charp, 0660);
MODULE_PARM_DESC(mp_strparam, "A demo string parameter");

static int __init mod_params_init(void)
{
    pr_info("%s: inserted succesfully\n", OUR_MODNAME);
    pr_info("module parameters are : mp_debug_level=%d mp_strparam=%s\n",
            mp_debug_level, mp_strparam);
    return 0;
}

static void __exit mod_params_exit(void)
{
    pr_info("module parameters are: mp_debug_level=%d mp_strparam=%s\n",
            mp_debug_level, mp_strparam);
    pr_info("%s: removed successfully\n", OUR_MODNAME);
}

module_init(mod_params_init);
module_exit(mod_params_exit);
