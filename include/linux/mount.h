/*
 *
 * Definitions for mount interface. This describes the in the kernel build 
 * linkedlist with mounted filesystems.
 *
 * Author:  Marco van Wieringen <mvw@planets.elm.net>
 *
 * Version: $Id: mount.h,v 2.0 1996/11/17 16:48:14 mvw Exp mvw $
 *
 */
#ifndef _LINUX_MOUNT_H
#define _LINUX_MOUNT_H
#ifdef __KERNEL__

#define MNT_VISIBLE	1

struct vfsmount
{
	//指向安装节点的dentry结构
	struct dentry *mnt_mountpoint;	/* dentry of mountpoint */
	//安装设备上的根目录dentry，即super block中指定的root dentry
	struct dentry *mnt_root;	/* root of the mounted tree */
	//上级设备的安装信息，即安装该文件系统前，该安装节点所在的文件系统
	struct vfsmount *mnt_parent;	/* fs we are mounted on */	
	//通过这个list_head链入被安装设备super block所维护的vsfmount队列中。
	struct list_head mnt_instances;	/* other vfsmounts of the same fs */
	//通过这个list_head连入被安装节点的dentry维护的vfsmount队列中。
	struct list_head mnt_clash;	/* those who are mounted on (other */
					/* instances) of the same dentry */
	//被安装到该节点的super block
	struct super_block *mnt_sb;	/* pointer to superblock */
	struct list_head mnt_mounts;	/* list of children, anchored here */
	struct list_head mnt_child;	/* and going through their mnt_child */
	atomic_t mnt_count;
	int mnt_flags;
	char *mnt_devname;		/* Name of device e.g. /dev/dsk/hda1 */
	//通过这个list_head链入内核维护的全局队列中，所有安装的文件系统都会链接到这个队列中统一管理
	struct list_head mnt_list;
	uid_t mnt_owner;
};

static inline struct vfsmount *mntget(struct vfsmount *mnt)
{
	if (mnt)
		atomic_inc(&mnt->mnt_count);
	return mnt;
}

static inline void mntput(struct vfsmount *mnt)
{
	if (mnt) {
		if (atomic_dec_and_test(&mnt->mnt_count))
			BUG();
	}
}

#endif
#endif /* _LINUX_MOUNT_H */
