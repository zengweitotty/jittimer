#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x2cb21faf, "module_layout" },
	{ 0xa90c928a, "param_ops_int" },
	{ 0xf09c7f68, "__wake_up" },
	{ 0x47c7b0d2, "cpu_number" },
	{ 0xe1bc7ede, "del_timer_sync" },
	{ 0xb00ccc33, "finish_wait" },
	{ 0xe75663a, "prepare_to_wait" },
	{ 0x4292364c, "schedule" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0x9df90e27, "current_task" },
	{ 0x71205378, "add_timer" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0x7d11c268, "jiffies" },
	{ 0xe96f9d2d, "create_proc_entry" },
	{ 0xe174aa7, "__init_waitqueue_head" },
	{ 0x9e1bdc28, "init_timer_key" },
	{ 0xaa5255e, "kmem_cache_alloc_trace" },
	{ 0x806ea81a, "kmalloc_caches" },
	{ 0x50eedeb8, "printk" },
	{ 0x6e6cd7d, "remove_proc_entry" },
	{ 0x37a0cba, "kfree" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "D55355437FCF10C2B3849C6");
