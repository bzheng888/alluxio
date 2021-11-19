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

#define _IOC_NRBITS   8
#define _IOC_TYPEBITS 8
#define _IOC_SIZEBITS 14
#define _IOC_DIRBITS  2

#define _IOC_NRMASK   ((1 << _IOC_NRBITS)-1)
#define _IOC_TYPEMASK ((1 << _IOC_TYPEBITS)-1)
#define _IOC_SIZEMASK ((1 << _IOC_SIZEBITS)-1)
#define _IOC_DIRMASK  ((1 << _IOC_DIRBITS)-1)
#define _IOC_NRSHIFT 0

#define _IOC_TYPESHIFT (_IOC_NRSHIFT+_IOC_NRBITS)
#define _IOC_SIZESHIFT (_IOC_TYPESHIFT+_IOC_TYPEBITS)
#define _IOC_DIRSHIFT  (_IOC_SIZESHIFT+_IOC_SIZEBITS)

#define _IOC_DIR(nr)  (((nr) >> _IOC_DIRSHIFT) & _IOC_DIRMASK)
#define _IOC_TYPE(nr) (((nr) >> _IOC_TYPESHIFT) & _IOC_TYPEMASK)
#define _IOC_NR(nr)   (((nr) >> _IOC_NRSHIFT) & _IOC_NRMASK)
#define _IOC_SIZE(nr) (((nr) >> _IOC_SIZESHIFT) & _IOC_SIZEMASK)

// Support cmd to operate fuse client.
#define CACHE_CLEAR     "clear"
#define CACHE_GET_SIZE  "get_size"

// Define a special file for ioctl.
#define IOC_SPECIAL_FILE  ".alluxio.cli.cmd"

#define MAXFUSEPATHLEN 128
#define MAXCMDLEN  32

typedef char relative_path[MAXFUSEPATHLEN];

// Ioctl cmd parameter definition.
enum {
  IOC_CACHE_SIZE	= _IOR('E', 0, size_t),
  IOC_CACHE_CLEAR	= _IOW('E', 1, relative_path),
};

class JniFuseClient {
public:
  JniFuseClient(const char *mnt_point, const char *cmd, const char *path) {
    m_path = new char[MAXFUSEPATHLEN];
    strcpy(m_path, path);
    m_cmd = new char[MAXCMDLEN];
    strcpy(m_cmd, cmd);
    m_mnt_point = new char[MAXFUSEPATHLEN];
    strcpy(m_mnt_point, mnt_point);
  }

  ~JniFuseClient() {
    delete [] m_mnt_point;
    delete [] m_path;
    delete [] m_cmd;
  }

  int exec_cmd() {
    int ret = 0;
    int fd;
    int size = 0;
    char special_file_path[MAXFUSEPATHLEN] = {0};

    if (m_mnt_point[strlen(m_mnt_point) - 1] != '/') {
      strcat(m_mnt_point, "/");
    }
    strcpy(special_file_path, m_mnt_point);
    strcat(special_file_path, IOC_SPECIAL_FILE);

    // First create a special file
  if ((fd = open(special_file_path, O_CREAT|O_WRONLY)) == -1) {
      perror("Open");
      return -1;
    }

    if (strcmp(m_cmd, CACHE_CLEAR) == 0) {
      if (ioctl(fd, IOC_CACHE_CLEAR, (void *)m_path)) {
        perror("ioctl");
        ret = -1;
        goto OUT;
      }
    }else if(strcmp(m_cmd, CACHE_GET_SIZE) == 0) {
      if (ioctl(fd, IOC_CACHE_SIZE, &size)) {
        perror("ioctl");
        ret = -1;
        goto OUT;
      }
      fprintf(stderr, "Cache size: %d\n", size);
    }else {
      fprintf(stderr, "Unsurpport cmd!");
      return -1;
    }
  OUT:
    close(fd);
    if (remove(special_file_path)) {
      perror("Remove");
    }
    return ret;
  }
private:
  char *m_path;
  char *m_cmd;
  char *m_mnt_point;
};

#endif // JNI_FUSE_CLI_H
