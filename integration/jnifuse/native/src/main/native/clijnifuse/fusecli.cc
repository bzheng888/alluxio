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

#include <memory>

#include "command.h"
#include "fusecli.h"

using namespace jnifuse;

std::map<std::string, COMMANDS> COMMANDS_MAP = {{"metadatacache", METADATACACHE}};

/**
 * Parses argv for the main function.
 */
class OptionParser {
 public:
  OptionParser(int& argc, char**& argv)
   : argc(argc), argv(argv), cmd(), subcommand()
   {
    argv++;
    argc--;
    if (argc < 1) {
     usage();
     exit(0);
    }

    Command* command;
    switch (COMMANDS_MAP[argv[0]]) {
     case METADATACACHE:
      command = new MetaDataCacheCommand();
      break;
     default:
      usage();
      exit(0);
    }

    cmd.reset(command);
    if (cmd->hasSubCommands()) {
     if (argc == 1) {
      std::cout << cmd->getUsage() << std::endl;
      exit(0);
     }
     argc--;
     argv++;
     std::map<std::string, Command*> subcommands = cmd->getSubCommands();
     if (subcommands.find(argv[0]) == subcommands.end()) {
      std::cout << cmd->getUsage() << std::endl;
      exit(0);
     }
     subcommand = subcommands[argv[0]];
    }
   }

   void usage() {
     std::cout
      << "A shell tool to get useful information f or fuse client. This program only supports linux OS."
      << std::endl
      << std::endl

      << "Usage:" << "fusecli [COMMAND] [COMMAND_ARGS]"
      << std::endl
      << std::endl

      << "COMMAND is one of:"
      << std::endl

      << "  metadatacache                "
      << "Command line tool for operate metadata cache."
      << std::endl
      << std::endl

      << "Commands print help when invoked without parameters."
      << std::endl;
    }

    int& argc;
    char**& argv;
    std::shared_ptr<Command> cmd;
    Command *subcommand;
};

int main(int argc, char **argv)
{
  OptionParser options(argc, argv);
  if (options.subcommand->hasOptions()) {
   CommandArgs args = options.subcommand->parseOptions(options.argc, options.argv);
  } else {

  }
  return 0;
}
