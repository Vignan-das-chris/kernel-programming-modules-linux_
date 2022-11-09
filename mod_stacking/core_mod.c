#define pr_fmt(fmt) "%s:%s(): " fmt, KBUILD_MODNAME, __func__

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#define MODNAME "core_mod"
#define THE_ONE 0xfedface
MODULE_LICENSE("Dual MIT/GPL");

int exp_int = 200;
EXPORT_SYMBOL_GPL(exp_int);

void sys_info2(void)
{
#define MSGLEN 128
    char msg[MSGLEN];

    memset(msg, 0, MSGLEN);
    snprintf(msg, 48, "%s(): Platform Info:\nCPU: ", __func__);

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
}
EXPORT_SYMBOL(sys_info2);

#if (BITS_PER_LONG == 32)
u32 get_skey(int p)
#else
u64 get_skey(int p)
#endif
{
#if (BITS_PER_LONG == 32)
    u32 secret = 0x567def;
#else // 64-bit
    u64 secret = 0x123abc567def;
#endif
    pr_info("%s:%d: I've been called\n", __FILE__, __LINE__);
    if (p == THE_ONE)
        return secret;
    return 0;
}
EXPORT_SYMBOL(get_skey);

static int __init core_mod_init(void)
{
    pr_info("insertion succesfull\n");
    return 0;
}

static void __exit core_mod_exit(void)
{
    pr_info("bits you added\n");
}

module_init(core_mod_init);
module_exit(core_mod_exit);
