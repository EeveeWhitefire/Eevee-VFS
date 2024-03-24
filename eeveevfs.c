#include <linux/init.h> // module_{init,exit}()
#include <linux/module.h> // THIS_MODULE, MODULE_VERSION, ...
#include <linux/kernel.h> // printk(), pr_*()
#include <asm/special_insns.h> // {read,write}_cr0()
#include <asm/processor-flags.h> // X86_CR0_WP
#include <asm/unistd.h> // __NR_*
#include <linux/kallsyms.h> // kallsyms_lookup_name()

/*
#ifdef pr_fmt
#undef pr_fmt
#endif
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
*/

#define LOG_PREFIX "EEVEE VFS -"

static sys_call_ptr_t* sys_call_table = kallsyms_lookup_name("sys_call_table");
static sys_call_ptr_t old_mkdir;

static sys_call_ptr_t eeveevfs_mkdir(const struct pt_regs* params)
{
	pr_info("%s mkdir executed! [%ld, 0x%lx, %lx]\n", LOG_PREFIX, params->di, params->si, params->dx);
	return old_mkdir(params);
}

static void edit_cr0(unsigned long val)
{
	asm volatile("mov %0,%%cr0": "+r" (val) : : "memory");
}

static int __init eeveevfs_begin()
{
	unsigned long old_cr0 = read_cr0();
	pr_info("%s Booting up!\n", LOG_PREFIX);

	pr_info("%s Hooking syscalls...\n", LOG_PREFIX);
	edit_cr0(old_cr0 & ~(X86_CR0_WP)); // disabling write-protection from syscall table

	// getting original syscall method ptrs
	old_mkdir = sys_call_table[__NR_mkdir];

	//updating to our own syscall methods
	sys_call_table[__NR_mkdir] = eeveevfs_mkdir;

	edit_cr0(old_cr0); // reenabling write-protection
	pr_info("%s Done!\n", LOG_PREFIX);

	return 0;
}

static void __exit eeveevfs_stop()
{
	unsigned long old_cr0 = read_cr0();
	
	pr_info("%s Returning syscall table back to normal...\n", LOG_PREFIX);
	edit_cr0(old_cr0 & ~(X86_CR0_WP)); // disabling write-protection from syscall table

	// getting syscall table back to normal
	sys_call_table[__NR_mkdir] = old_mkdir;

	edit_cr0(old_cr0); // reenabling write-protection
	pr_info("%s Shutting down!\n", LOG_PREFIX);
}


module_init(eeveevfs_begin);
module_exit(eeveevfs_stop);
MODULE_VERSION("0.1");
MODULE_AUTHOR("Tal Hadad");
MODULE_LICENSE("MIT")
