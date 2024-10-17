# 3-1
When using the Linux-specific reboot() system call to reboot the system, the second argument, magic2, must be specified as one of a set of magic numbers (e.g., LINUX_REBOOT_MAGIC2). What is the significance of these numbers? (Converting them to hexadecimal provides a clue.)

## answer
in my computer, the magic numbers are in `/usr/include/linux/reboot.h`:
```
cat /usr/include/linux/reboot.h | grep LINUX_REBOOT_MAGIC 
```

```
#define	LINUX_REBOOT_MAGIC1	0xfee1dead
#define	LINUX_REBOOT_MAGIC2	672274793
#define	LINUX_REBOOT_MAGIC2A	85072278
#define	LINUX_REBOOT_MAGIC2B	369367448
#define	LINUX_REBOOT_MAGIC2C	537993216
```

get hex:
```
grep LINUX_REBOOT_MAGIC /usr/include/linux/reboot.h | awk '{if($3 ~ /^[0-9]+$/) printf "%s %s 0x%x\n", $1, $2, $3; else print}'
```

```
#define LINUX_REBOOT_MAGIC1     0xfee1dead
#define LINUX_REBOOT_MAGIC2 0x28121969
#define LINUX_REBOOT_MAGIC2A 0x5121996
#define LINUX_REBOOT_MAGIC2B 0x16041998
#define LINUX_REBOOT_MAGIC2C 0x20112000
```

hex number meaning: %D-%M-%Y

28-12-1969 is linus's birthday.Other numbers are his 3 daughters' birthdays.

