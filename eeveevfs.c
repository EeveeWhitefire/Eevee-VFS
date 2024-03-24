#include <linux/init.h> // module_{init,exit}()
#include <linux/module.h> // THIS_MODULE, MODULE_VERSION, ...
#include <linux/kernel.h> // printk(), pr_*()
#include <asm/special_insns.h> // {read,write}_cr0()
#include <asm/processor-flags.h> // X86_CR0_WP
#include <asm/unistd.h> // __NR_*


#ifdef pr_fmt
#undef pr_fmt
#endif
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#define LOG_PREFIX "EEVEE VFS -"

static int __init eeveevfs_begin()
{
	pr_info("%s Shutting down!\n", LOG_PREFIX);
	return 0;
}

static void __exit eeveevfs_stop()
{
	pr_info("%s Shutting down!\n", LOG_PREFIX);
}


module_init(eeveevfs_begin);
module_exit(eeveevfs_stop);
MODULE_VERSION("0.1");
MODULE_AUTHOR("Tal Hadad");
MODULE_LICENSE("MIT")
