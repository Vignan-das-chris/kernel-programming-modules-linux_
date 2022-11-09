#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>

#define OURMODNAME "slab_a"

MODULE_AUTHOR("Vignan das");
MODULE_DESCRIPTION("using k[m|z]alloc, kfree, basic demo");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

static char *gkptr;
struct myctx
{
    u32 iarr[100];
    u64 uarr[100];
    char uname[128], passwd[16], config[16];
};
static struct myctx *ctx;

static int __init slab_a_init(void)
{
    gkptr = kmalloc(1024, GFP_KERNEL);
    if (!gkptr)
    {
        WARN_ONCE(1, "%s: kmalloc() failed!\n", OURMODNAME);
        goto out_fail1;
    }
    pr_info("kmalloc() succeeds, (actual KVA) ret value = %px\n", gkptr);
    print_hex_dump_bytes("gkptr before memset: ", DUMP_PREFIX_OFFSET, gkptr, 32);
    memset(gkptr, 'm', 1024);
    print_hex_dump_bytes(" gkptr after memset: ", DUMP_PREFIX_OFFSET, gkptr, 32);

    ctx = kzalloc(sizeof(struct myctx), GFP_KERNEL);
    if (!ctx)
        goto out_fail2;
    pr_info("%s: context struct alloc'ed and initialized (actual KVA ret = %px)\n",
            OURMODNAME, ctx);
    print_hex_dump_bytes("ctx: ", DUMP_PREFIX_OFFSET, ctx, 32);

    return 0;

out_fail2:
    kfree(gkptr);
out_fail1:
    return -ENOMEM;
}

static void __exit slab_a_exit(void)
{
    kfree(ctx);
    kfree(gkptr);
    pr_info("%s: freed slab memory, removed\n", OURMODNAME);
}

module_init(slab_a_init);
module_exit(slab_a_exit);
