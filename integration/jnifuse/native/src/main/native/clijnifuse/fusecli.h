/*
 * The Alluxio Open Foundation licenses this work under the Apache License, version 2.0
 * (the "License"). You may not use this work except in compliance with the License, which is
 * available at www.apache.org/licenses/LICENSE-2.0
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied, as more fully set forth in the License.
 *
 * See the NOTICE file distributed with this work for information regarding copyright ownership.
 */

#ifndef JNI_FUSE_CLI_H
#define JNI_FUSE_CLI_H

#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

// Define a tmp file for ioctl.
#define IOC_TMP_FILE  ".alluxio.cli"

#define MAXFUSEPATHLEN 128
#define MAXCMDLEN  32

typedef char relative_path[MAXFUSEPATHLEN];

// Ioctl cmd parameter definition.
enum {
  IOC_CACHE_SIZE	= _IOR('E', 0, size_t),
  IOC_CACHE_CLEAR	= _IOW('E', 1, relative_path),
};

enum COMMANDS {
 INVALID,
 METADATACACHE
};

#endif // JNI_FUSE_CLI_H
