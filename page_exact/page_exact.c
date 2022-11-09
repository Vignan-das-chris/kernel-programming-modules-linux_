#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>

#define OURMODNAME "page_exact"

MODULE_AUTHOR("Vignan das");
MODULE_DESCRIPTION(" using the pages_exact() APIs");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

#define MAXTIMES
void show_phy_pages(const void *kaddr, size_t len, bool contiguity_check);

static void *gptr[MAXTIMES];
static size_t gsz = 33 * 1024;

static int __init page_exact_init(void)
{
    int i, j;

    pr_info("%s: inserted\n", OURMODNAME);

    for (i = 0; i < MAXTIMES; i++)
    {
        gptr[i] = alloc_pages_exact(gsz, GFP_KERNEL);
        if (!gptr[i])
        {
            pr_warn("%s: alloc_pages_exact() failed! (loop index %d)\n",
                    OURMODNAME, i);
            /* it failed; don't leak, ensure we free the memory taken so far! */
            for (j = i; j >= 0; j--)
                free_pages_exact(gptr[j], gsz);
            return -ENOMEM;
        }
        pr_info("%s:%d: alloc_pages_exact() alloc'ed %zu bytes memory (%zu pages + rem %u bytes)"
                " from the BSA @ %pK (actual=%px)\n",
                OURMODNAME, i, gsz, gsz / PAGE_SIZE, gsz % PAGE_SIZE, gptr[i], gptr[i]);
        // lets 'poison' it..
        memset(gptr[i], 'x', gsz);

        show_phy_pages(gptr[i], gsz, 1);
        msleep(100);
    }

    return 0;
}

static void __exit page_exact_exit(void)
{
    int i;

    for (i = 0; i < MAXTIMES; i++)
        free_pages_exact(gptr[i], gsz);
    pr_info("%s: mem freed, removed\n", OURMODNAME);
}

module_init(page_exact_init);
module_exit(page_exact_exit);
