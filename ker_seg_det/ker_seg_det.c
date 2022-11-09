#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/highmem.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/version.h>
#include <asm/pgtable.h>
#include <asm/fixmap.h>
#include "../../klib_llkd.h"
#include "../../convenient.h"

#define OURMODNAME "ker_seg_det"

MODULE_AUTHOR("Vignan_das");
MODULE_DESCRIPTION("display  kernel segment details");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

static int show_uservas;
module_param(show_uservas, int, 0660);
MODULE_PARM_DESC(show_uservas, "Show some user space VAS details; 0 = no (default), 1 = show");

#define ELLPS "|                           [ . . . ]                         |\n"

extern void sysinfo(void);

static void show_userspace_info(void)
{
    pr_info("+------------ Above is kernel-seg; below, user VAS  ----------+\n" ELLPS
            "|Process environment "
            " %px - %px     | [ %4zd bytes]\n"
            "|          arguments "
            " %px - %px     | [ %4zd bytes]\n"
            "|        stack start  %px\n"
            "|       heap segment "
            " %px - %px     | [ %4zd KB]\n"
            "|static data segment "
            " %px - %px     | [ %4zd bytes]\n"
            "|       text segment "
            " %px - %px     | [ %4zd KB]\n" ELLPS
            "+-------------------------------------------------------------+\n",
            SHOW_DELTA_b(current->mm->env_start, current->mm->env_end),
            SHOW_DELTA_b(current->mm->arg_start, current->mm->arg_end),
            current->mm->start_stack,
            SHOW_DELTA_K(current->mm->start_brk, current->mm->brk),
            SHOW_DELTA_b(current->mm->start_data, current->mm->end_data),
            SHOW_DELTA_K(current->mm->start_code, current->mm->end_code));

    pr_info(
#if (BITS_PER_LONG == 64)
        "Above: TASK_SIZE         = %llu size of userland  [  %ld GB]\n"
#else
        "Above: TASK_SIZE         = %lu size of userland  [  %ld MB]\n"
#endif
        " # userspace memory regions (VMAs) = %d\n"
        " Above statistics are wrt 'current' thread (see below):\n",
#if (BITS_PER_LONG == 64)
        TASK_SIZE, (TASK_SIZE >> 30),
#else // 32-bit
        TASK_SIZE, (TASK_SIZE >> 20),
#endif
        current->mm->map_count);

    PRINT_CTX();
}

static void show_kernelseg_info(void)
{
    pr_info("\nSome Kernel Details [by decreasing address]\n"
            "+-------------------------------------------------------------+\n");
#ifdef CONFIG_ARM
    /* On ARM, the definition of VECTORS_BASE turns up only in kernels >= 4.11 */
#if LINUX_VERSION_CODE > KERNEL_VERSION(4, 11, 0)
    pr_info("|vector table:       "
            " %px - %px | [%4ld KB]\n",
            SHOW_DELTA_K(VECTORS_BASE, VECTORS_BASE + PAGE_SIZE));
#endif
#endif

    pr_info(ELLPS
            "|fixmap region:      "
            " %px - %px     | [%4ld MB]\n",
#ifdef CONFIG_ARM
            SHOW_DELTA_M(FIXADDR_START, FIXADDR_END));
#else
            SHOW_DELTA_M(FIXADDR_START, (FIXADDR_START + FIXADDR_SIZE)));
#endif

#if (BITS_PER_LONG == 64)
    pr_info("|module region:      "
            " %px - %px     | [%ld MB]\n",
            SHOW_DELTA_M(MODULES_VADDR, MODULES_END));
#endif

#ifdef CONFIG_KASAN
    pr_info("|KASAN shadow:       "
            " %px - %px     | [%2ld GB]\n",
            SHOW_DELTA_G(KASAN_SHADOW_START, KASAN_SHADOW_END));
#endif

    pr_info("|vmalloc region:     "
            " %px - %px     | [%4ld MB = %2ld GB]\n",
            SHOW_DELTA_MG(VMALLOC_START, VMALLOC_END));

    pr_info("|lowmem region:      "
            " %px - %px     | [%4ld MB = %2ld GB]\n"
#if (BITS_PER_LONG == 32)
            "|           (above:PAGE_OFFSET - highmem)                     |\n",
#else
            "|                (above:PAGE_OFFSET    -      highmem)        |\n",
#endif
            SHOW_DELTA_MG((unsigned long)PAGE_OFFSET, (unsigned long)high_memory));

#ifdef CONFIG_HIGHMEM
    pr_info("|HIGHMEM region:     "
            " %px - %px | [%4ld MB]\n",
            SHOW_DELTA_M(PKMAP_BASE, (PKMAP_BASE) + (LAST_PKMAP * PAGE_SIZE)));
#endif

#if (BITS_PER_LONG == 32)
    pr_info("|module region:      "
            " %px - %px | [%4ld MB]\n",
            SHOW_DELTA_M(MODULES_VADDR, MODULES_END));
#endif
    pr_info(ELLPS);
}

static int __init ker_seg_init(void)
{
    pr_info("%s: inserted\n", OURMODNAME);

    llkd_minsysinfo();
    show_kernelseg_info();

    if (show_uservas)
        show_userspace_info();
    else
    {
        pr_info("+-------------------------------------------------------------+\n");
        pr_info("%s: skipping show userspace...\n", OURMODNAME);
    }

    return 0; /* success */
}

static void __exit ker_seg_exit(void)
{
    pr_info("%s: removed\n", OURMODNAME);
}

module_init(ker_seg_init);
module_exit(ker_seg_exit);
Footer
