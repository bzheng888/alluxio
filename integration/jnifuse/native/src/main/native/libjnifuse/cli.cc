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

#include "cli.h"

const char *usage =
"Usage: cli mountpint Command [relative path]\n"
"Command:\n"
"\tclear: Clear metadata cache for the give path,\n"
"\tget_size: Get the client metadata cache size.\n"
"Example:\n"
"\tcli /tmp/alluxio-fuse/ clear /\n"
"\tcli /tmp/alluxio-fuse/ get_size\t"
"\n";

int main(int argc, char **argv)
{
  JniFuseClient *cli = nullptr;
  char mnt_point[MAXFUSEPATHLEN] = {0};
  char path[MAXFUSEPATHLEN] = {0};
  char cmd[MAXCMDLEN] = {0};
  if (argc < 3) {
      fprintf(stderr, "%s", usage);
      return -1;
  }
  strncpy(mnt_point, argv[1], strlen(argv[1]) + 1);
  strncpy(cmd, argv[2], strlen(argv[2]) + 1);
  if (argc == 4) {
      strncpy(path, argv[3], strlen(argv[3]) + 1);
  }
  cli = new JniFuseClient(mnt_point, cmd, path);
  cli->exec_cmd();
  delete cli;
  return 0;
}
