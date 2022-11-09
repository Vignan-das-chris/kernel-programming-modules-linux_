#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/limits.h>

#define MYMODNAME "sys_info"
MODULE_AUTHOR("Vignan Das");
MODULE_DESCRIPTION("to give some information about the system");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

void sizeof_types(void)
{
    pr_info("sizeof: (bytes)\n"
            "  char = %2zu   short int = %2zu              int = %2zu\n"
            "  long = %2zu   long long = %2zu           void * = %2zu\n"
            " float = %2zu      double = %2zu      long double = %2zu\n",
            sizeof(char), sizeof(short int),
            sizeof(int), sizeof(long), sizeof(long long), sizeof(void *),
            sizeof(float), sizeof(double), sizeof(long double));
}

void sys_info3(void)
{
#define MSGLEN 128
    char msg[MSGLEN];

    memset(msg, 0, MSGLEN);
    snprintf(msg, 48, "%s(): minimal Platform Info:\nCPU: ", __func__);

#ifdef CONFIG_X86
#if (BITS_PER_LONG == 32)
    strlcat(msg, "x86_32, ", MSGLEN);
#else
    strlcat(msg, "x86_64, ", MSGLEN);
#endif
#endif
#ifdef CONFIG_ARM
    strlcat(msg, "ARM-32, ", MSGLEN);
#endif
#ifdef CONFIG_ARM64
    strlcat(msg, "Aarch64, ", MSGLEN);
#endif
#ifdef CONFIG_MIPS
    strlcat(msg, "MIPS, ", MSGLEN);
#endif
#ifdef CONFIG_PPC
    strlcat(msg, "PowerPC, ", MSGLEN);
#endif
#ifdef CONFIG_S390
    strlcat(msg, "IBM S390, ", MSGLEN);
#endif

#ifdef __BIG_ENDIAN
    strlcat(msg, "big-endian; ", MSGLEN);
#else
    strlcat(msg, "little-endian; ", MSGLEN);
#endif

#if (BITS_PER_LONG == 32)
    strlcat(msg, "32-bit OS.\n", MSGLEN);
#elif (BITS_PER_LONG == 64)
    strlcat(msg, "64-bit OS.\n", MSGLEN);
#endif
    pr_info("%s", msg);

    sizeof_types();

    pr_info("Word [U|S][8|16|32|64] ranges: unsigned max, signed max, signed min:\n"
            " U8_MAX = %20u = 0x%16x,  S8_MAX = %20d = 0x%16x,  S8_MIN = %20d = 0x%16x\n"
            "U16_MAX = %20u = 0x%16x, S16_MAX = %20d = 0x%16x, S16_MIN = %20d = 0x%16x\n"
            "U32_MAX = %20u = 0x%16x, S32_MAX = %20d = 0x%16x, S32_MIN = %20d = 0x%16x\n"
            "U64_MAX = %20llu = 0x%16llx, S64_MAX = %20lld = 0x%16llx, S64_MIN = %20lld = 0x%16llx\n"

#if (BITS_PER_LONG == 32)
            "PHYS_ADDR_MAX = %u = 0x%px\n"
#else
            "PHYS_ADDR_MAX = %llu = 0x%px\n"
#endif
            ,
            U8_MAX, U8_MAX, S8_MAX, S8_MAX, S8_MIN, S8_MIN,
            U16_MAX, U16_MAX, S16_MAX, S16_MAX, S16_MIN, S16_MIN,
            U32_MAX, U32_MAX, S32_MAX, S32_MAX, S32_MIN, S32_MIN,
            U64_MAX, U64_MAX, S64_MAX, S64_MAX, S64_MIN, S64_MIN, PHYS_ADDR_MAX, (void *)PHYS_ADDR_MAX);
}
EXPORT_SYMBOL(sys_info3);

void sys_info2(void)
{
    char msg[128];

    memset(msg, 0, strlen(msg));
    snprintf(msg, 47, "%s(): minimal Platform Info:\nCPU: ", __func__);

#ifdef CONFIG_X86
#if (BITS_PER_LONG == 32)
    strncat(msg, "x86_32, ", 9);
#else
    strncat(msg, "x86_64, ", 9);
#endif
#endif
#ifdef CONFIG_ARM
    strncat(msg, "ARM-32, ", 9);
#endif
#ifdef CONFIG_ARM64
    strncat(msg, "Aarch64, ", 10);
#endif
#ifdef CONFIG_MIPS
    strncat(msg, "MIPS, ", 7);
#endif
#ifdef CONFIG_PPC
    strncat(msg, "PowerPC, ", 10);
#endif
#ifdef CONFIG_S390
    strncat(msg, "IBM S390, ", 11);
#endif

#ifdef __BIG_ENDIAN
    strncat(msg, "big-endian; ", 13);
#else
    strncat(msg, "little-endian; ", 16);
#endif

#if (BITS_PER_LONG == 32)
    strncat(msg, "32-bit OS.\n", 12);
#elif (BITS_PER_LONG == 64)
    strncat(msg, "64-bit OS.\n", 12);
#endif
    pr_info("%s", msg);
}
EXPORT_SYMBOL(sys_info2);

static int __init sys_info_init(void)
{
    pr_info("%s: inserted\n", MYMODNAME);
    sys_info2();
    sys_info3();
    return 0;
}

static void __exit sys_info_exit(void)
{
    pr_info("%s: removed\n", MYMODNAME);
}

module_init(sys_info_init);
module_exit(sys_info_exit);
