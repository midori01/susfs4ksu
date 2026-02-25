#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/reboot.h>
#include <sys/syscall.h>
#include <errno.h>
#include "def.h"
#include "utils.h"
#include "sus_path.h"

struct st_susfs_sus_path {
	unsigned long           target_ino;
	char                    target_pathname[SUSFS_MAX_LEN_PATHNAME];
	unsigned int            i_uid;
	int                     err;
};

void sus_path_print_help(void){
	log("    add_sus_path </path/of/file_or_directory>\n");
	log("      |--> Added path and all its sub-paths will be hidden for umounted app process from several syscalls\n");
	log("      |--> Please be reminded that the target path must be added after the bind mount or overlay operation if any, otherwise it won't be effective\n");
	log("\n");
	log("    add_sus_path_loop </path/not/inside/sdcard>\n");
	log("      |--> The only difference to add_sus_path is that the added sus_path via this cli will be flagged as SUS_PATH again for the app process when it is being spawned by zygote and marked umounted\n");
	log("\n");
}

static void print_help(void){
	print_help_banner();
	sus_path_print_help();
}

int add_sus_path(int argc, char *argv[]) {
	struct st_susfs_sus_path info = {0};
	struct stat sb;

	if (argc != 3) {
		print_help();
		return -EINVAL;
	}

	info.err = get_file_stat(argv[2], &sb);
	if (info.err) {
		log("[-] failed to get stat from path: '%s'\n", argv[2]);
		return info.err;
	}
	strncpy(info.target_pathname, argv[2], SUSFS_MAX_LEN_PATHNAME-1);
	info.target_ino = sb.st_ino;
	info.i_uid = sb.st_uid;
	info.err = ERR_CMD_NOT_SUPPORTED;
	syscall(SYS_reboot, KSU_INSTALL_MAGIC1, SUSFS_MAGIC, CMD_SUSFS_ADD_SUS_PATH, &info);
	PRT_MSG_IF_CMD_NOT_SUPPORTED(info.err, CMD_SUSFS_ADD_SUS_PATH);
	return info.err;
}

int add_sus_path_loop(int argc, char *argv[]) {
	struct st_susfs_sus_path info = {0};
	struct stat sb;

	if (argc != 3) {
		print_help();
		return -EINVAL;
	}

	info.err = get_file_stat(argv[2], &sb);
	if (info.err) {
		log("[-] failed to get stat from path: '%s'\n", argv[2]);
		return info.err;
	}
	strncpy(info.target_pathname, argv[2], SUSFS_MAX_LEN_PATHNAME-1);
	info.target_ino = sb.st_ino;
	info.i_uid = sb.st_uid;
	info.err = ERR_CMD_NOT_SUPPORTED;
	syscall(SYS_reboot, KSU_INSTALL_MAGIC1, SUSFS_MAGIC, CMD_SUSFS_ADD_SUS_PATH_LOOP, &info);
	PRT_MSG_IF_CMD_NOT_SUPPORTED(info.err, CMD_SUSFS_ADD_SUS_PATH_LOOP);
	return info.err;
}
