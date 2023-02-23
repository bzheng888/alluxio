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

#ifndef FUSE_NATIVE_CLI_COMMAND_H_
#define FUSE_NATIVE_CLI_COMMAND_H_

#include <getopt.h>
#include <iostream>
#include <map>
#include <stdlib.h>
#include <string>
#include <unistd.h>

namespace jnifuse {

#define DEFAULT_MOUNT_POINT "/tmp/alluxio-fuse"

class MetaDataCacheCommand;
class ClearAllMetadataCache;
class ClearMetaDataCache;
class MetadataCacheSize;

struct CommandArgs {
 std::string mountPoint;
 std::string path;
};

class Command {
 public:
  virtual std::string getUsage() const = 0;
  virtual std::string getCommandName () const = 0;
  virtual bool hasSubCommands () const = 0;
  virtual std::map<std::string, Command*> getSubCommands() const = 0;
  virtual bool hasOptions() const = 0;
  // Parse options, the return value should expend into struct.
  virtual CommandArgs parseOptions(int& argc, char**& argv) = 0;
 protected:
  std::map<std::string, Command*> subCommands;
  CommandArgs args;
};

class ClearMetaDataCache : public Command {
 public:
   std::string getCommandName() const override {
    return "clear";
   }
   std::string getUsage() const override {
    return "fusecli metadatacache clear [options]" \
           "options:"
           "\t-m, --mnt" \
           "                The fuse mount point, default /tmp/alluxio-fuse.";
           "\t-p, --path" \
           "                Clear the metadata cache for the specific path.";
   }
   bool hasSubCommands() const override {
    return false;
   }
   bool hasOptions() const override {
    return true;
   }
   std::map<std::string, Command*> getSubCommands() const override {
    return subCommands;
   }
   CommandArgs parseOptions(int& argc, char**& argv) override;
};

CommandArgs ClearMetaDataCache::parseOptions(int& argc, char**& argv) {
 while(true) {
  static struct option longOptions[] = {
   {"path",  required_argument, NULL, 'p'}
  };
  int c = getopt_long(argc, argv, "p:", longOptions, NULL);
  if (c == -1) break;
  switch (c){
   case 'p':
    args.path = optarg;
    break;
   default:
    getUsage();
    exit(0);
  }
 }
 if (optind != argc) {
  getUsage();
  exit(0);
 }
 return args;
}

class ClearAllMetadataCache : public Command {
 public:
   std::string getCommandName() const override {
    return "clearAll";
   }
   std::string getUsage() const override {
    return "clearAll" \
           "                Clear all the metadata cache on the client.";
   }
   bool hasSubCommands() const override {
    return false;
   }
   bool hasOptions() const override {
    return false;
   }
   std::map<std::string, Command*> getSubCommands() const override {
    return subCommands;
   }
   CommandArgs parseOptions(int& argc, char**& argv) override {
    return args;
   }
};

class MetadataCacheSize : public Command {
 public:
   std::string getCommandName() const override {
    return "size";
   }
   std::string getUsage() const override {
    return "size [options]" \
           "options:"
           "\t-m, --mnt" \
           "                The fuse mount point, default /tmp/alluxio-fuse.";
           "\t-p, --path" \
           "                get the metadata size for the specific path.";
   }
   bool hasSubCommands() const override {
    return false;
   }
   bool hasOptions() const override {
    return true;
   }
   std::map<std::string, Command*> getSubCommands() const override {
    return subCommands;
   }
   CommandArgs parseOptions(int& argc, char**& argv) override;
};

CommandArgs MetadataCacheSize::parseOptions(int& argc, char**& argv) {
 while(true) {
  static struct option longOptions[] = {
   {"path",  required_argument, NULL, 'p'}
  };
  int c = getopt_long(argc, argv, "p:", longOptions, NULL);
  if (c == 1) break;
  switch (c){
   case 'p':
    args.path = optarg;
    break;
   default:
    getUsage();
    exit(0);
  }
 }
 if (optind != argc) {
  getUsage();
  exit(0);
 }
 return args;
}

class MetaDataCacheCommand : public Command {
 public:
  MetaDataCacheCommand() {
   subCommands.insert(std::map<std::string, Command*>::value_type("clear", new ClearMetaDataCache()));
   subCommands.insert(std::map<std::string, Command*>::value_type("clearAll", new ClearAllMetadataCache()));
   subCommands.insert(std::map<std::string, Command*>::value_type("size", new MetadataCacheSize()));
  }
  ~MetaDataCacheCommand() {
   for (const auto& cmd:subCommands) {
    if (cmd.second != NULL)
     delete cmd.second;
   }
  }

  std::string getUsage() const override;
  std::string getCommandName() const override;
  bool hasSubCommands() const override;
  bool hasOptions() const override {
   return false;
  }
  std::map<std::string, Command*> getSubCommands() const override;
  CommandArgs parseOptions(int& argc, char**& argv) override {
    return args;
  }
};

std::string MetaDataCacheCommand::getCommandName() const {
 return "metadatacache";
}

bool MetaDataCacheCommand::hasSubCommands() const {
 return true;
}
std::map<std::string, Command*> MetaDataCacheCommand::getSubCommands() const {
 return subCommands;
}

std::string MetaDataCacheCommand::getUsage() const {
 std::string usage = "Usage: fusecli metadatacache";
 for (const auto& cmd : subCommands) {
   usage.append(" [").append(cmd.first).append("] ");
 }
 usage.append("\nProvide operations for the metadata. See sub-commands' descriptions for more details.");
 return usage;
}
}

#endif // FUSE_NATIVE_CLI_COMMAND_H_