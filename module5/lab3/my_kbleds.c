#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>
#include <linux/tty.h>
#include <linux/vt.h>
#include <linux/vt_kern.h>
#include <linux/kd.h>
#include <linux/console_struct.h>

#define NAME "my_kbleds"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ivan Yashkov");
MODULE_DESCRIPTION("Keyboard LEDs manipulations through mask in sysfs");

static struct kobject *kobj;
static int leds_mask = 0;

static void set_leds_mask(void) {
  struct tty_struct *tty;

  int temp = 0;
  if (leds_mask & 1) temp |= 1;
  if (leds_mask & 2) temp |= 4;
  if (leds_mask & 4) temp |= 2;
  leds_mask = temp;

  tty = tty_port_tty_get(&vc_cons[fg_console].d->port);
  if (tty) {
    tty->driver->ops->ioctl(tty, KDSETLED, leds_mask);
    tty_kref_put(tty);
  }
}

static ssize_t leds_mask_show(struct kobject *kobj, struct kobj_attribute *attr,
                              char *buf) {
  return sprintf(buf, "%d\n", leds_mask);
}

static ssize_t leds_mask_store(struct kobject *kobj,
                               struct kobj_attribute *attr, const char *buf,
                               size_t count) {
  unsigned int new_leds_mask;
  int ret = sscanf(buf, "%u", &new_leds_mask);
  if (ret != 1) return -EINVAL;

  if (new_leds_mask < 0 || new_leds_mask > 7) return -EINVAL;
  leds_mask = new_leds_mask;
  set_leds_mask();

  return count;
}

static struct kobj_attribute leds_mask_attribute =
    __ATTR(leds_mask, 0644, leds_mask_show, leds_mask_store);

static int __init kbleds_init(void) {
  int error;
  pr_info(NAME ": INFO - start\n");

  kobj = kobject_create_and_add(NAME, kernel_kobj);
  if (!kobj) return -ENOMEM;

  error = sysfs_create_file(kobj, &leds_mask_attribute.attr);
  if (error) {
    pr_err(NAME ": ERROR - cannot create file in sys\n");
    return error;
  }

  pr_info(NAME ": INFO - file in sys is created: /sys/kernel/" NAME
               "/leds_mask\n");
  return 0;
}

static void __exit kbleds_exit(void) {
  pr_info(NAME ": INFO - exit\n");
  kobject_put(kobj);
}

module_init(kbleds_init);
module_exit(kbleds_exit);
