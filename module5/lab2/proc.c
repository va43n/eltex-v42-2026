#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define PROC_NAME "hello"
#define MSG_SIZE 10

MODULE_LICENSE("eltex-v42-2026");
MODULE_AUTHOR("Ivan Yashkov");
MODULE_DESCRIPTION("Info exchange through proc");

static int len, temp;

static char *msg;

static ssize_t read_proc(struct file *filp, char *buf, size_t count,
                         loff_t *offp) {
  if (count > temp) {
    count = temp;
  }
  temp = temp - count;
  if (copy_to_user(buf, msg, count)) return -EFAULT;
  if (count == 0) temp = len;

  return count;
}

static ssize_t write_proc(struct file *filp, const char *buf, size_t count,
                          loff_t *offp) {
  if (copy_from_user(msg, buf, count)) return -EFAULT;
  len = count;
  temp = len;

  return count;
}

static struct proc_ops proc_fops = {
  proc_read : read_proc,
  proc_write : write_proc
};

static void create_new_proc_entry(void) {
  proc_create(PROC_NAME, 0666, NULL, &proc_fops);
  msg = kmalloc(MSG_SIZE * sizeof(char), GFP_KERNEL);
}

static int __init proc_init(void) {
  printk(KERN_INFO "proc.ko: proc_init - started\n");

  create_new_proc_entry();
  return 0;
}

static void __exit proc_cleanup(void) {
  printk(KERN_INFO "proc.ko: proc_cleanup - cleaning\n");

  remove_proc_entry(PROC_NAME, NULL);
  kfree(msg);
}

module_init(proc_init);
module_exit(proc_cleanup);