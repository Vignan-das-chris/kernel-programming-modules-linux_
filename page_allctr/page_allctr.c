#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include "../../klib_lkms.h"

#define OURMODNAME "page_allctr"

MODULE_DESCRIPTION("page allocator apis");
MODULE_AUTHOR("Vignan das");
MODULE_LICENSE("Dual MIT/GPL");

static const void *gptr1, *gptr2, *gptr3, *gptr4, *gptr5;
static int bsa_alloc_order = 3;
module_param_named(order, bsa_alloc_order, int, 0660);
MODULE_PARM_DESC(order, "order of the allocation (power-to-raise-2-to)");

static int bsa_alloc(void)
{
    int stat = -ENOMEM;
    u64 numpg2alloc = 0;
    const struct page *pg_ptr1;

    pr_info("%s: 0. Show identity mapping: RAM page frames : kernel virtual pages :: 1:1\n",
            OURMODNAME);
    show_phy_pages((void *)PAGE_OFFSET, 5 * PAGE_SIZE, 1);

    gptr1 = (void *)__get_free_page(GFP_KERNEL);
    if (!gptr1)
    {
        pr_warn("%s: __get_free_page() failed!\n", OURMODNAME);
        goto out1;
    }
    pr_info("%s: 1. __get_free_page() alloc'ed 1 page from the BSA @ %pK (%px)\n",
            OURMODNAME, gptr1, gptr1);

    numpg2alloc = powerof(2, bsa_alloc_order); // returns 2^bsa_alloc_order
    gptr2 = (void *)__get_free_pages(GFP_KERNEL | __GFP_ZERO, bsa_alloc_order);
    if (!gptr2)
    {
        goto out2;
    }
    pr_info("%s: 2. __get_free_pages() alloc'ed 2^%d = %lld page(s) = %lld bytes\n"
            " from the BSA @ %pK (%px)\n",
            OURMODNAME, bsa_alloc_order, powerof(2, bsa_alloc_order),
            numpg2alloc * PAGE_SIZE, gptr2, gptr2);
    pr_info(" (PAGE_SIZE = %ld bytes)\n", PAGE_SIZE);
    show_phy_pages(gptr2, numpg2alloc * PAGE_SIZE, 1);

    gptr3 = (void *)get_zeroed_page(GFP_KERNEL);
    if (!gptr3)
        goto out3;
    pr_info("%s: 3. get_zeroed_page() alloc'ed 1 page from the BSA @ %pK (%px)\n",
            OURMODNAME, gptr3, gptr3);

    pg_ptr1 = alloc_page(GFP_KERNEL);
    if (!pg_ptr1)
        goto out4;
    gptr4 = page_address(pg_ptr1);
    pr_info("%s: 4. alloc_page() alloc'ed 1 page from the BSA @ %pK (%px)\n"
            " (struct page addr=%pK (%px))\n",
            OURMODNAME, (void *)gptr4, (void *)gptr4, pg_ptr1, pg_ptr1);

    gptr5 = page_address(alloc_pages(GFP_KERNEL, 5));
    if (!gptr5)
        goto out5;
    pr_info("%s: 5. alloc_pages() alloc'ed %lld pages from the BSA @ %pK (%px)\n",
            OURMODNAME, powerof(2, 5), (void *)gptr5, (void *)gptr5);

    return 0;
out5:
    free_page((unsigned long)gptr4);
out4:
    free_page((unsigned long)gptr3);
out3:
    free_pages((unsigned long)gptr2, bsa_alloc_order);
out2:
    free_page((unsigned long)gptr1);
out1:
    return stat;
}
static int __init page_allctr_init(void)
{
    return bsa_alloc();
}

static void __exit page_allctr_exit(void)
{
    pr_info("%s: free-ing up the BSA memory chunks...\n", OURMODNAME);
    free_pages((unsigned long)gptr5, 3);
    free_page((unsigned long)gptr4);
    free_page((unsigned long)gptr3);
    free_pages((unsigned long)gptr2, bsa_alloc_order);
    free_page((unsigned long)gptr1);
    pr_info("%s: removed\n", OURMODNAME);
}

module_init(page_allctr_init);
module_exit(page_allctr_exit);
