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

package alluxio.cli.fsadmin.decommissionworkers;

import alluxio.cli.fsadmin.FileSystemAdminShellUtils;
import alluxio.cli.fsadmin.command.AbstractFsAdminCommand;
import alluxio.cli.fsadmin.command.Context;
import alluxio.conf.AlluxioConfiguration;
import alluxio.exception.status.AlluxioStatusException;

import org.apache.commons.cli.CommandLine;
import org.apache.commons.cli.Option;
import org.apache.commons.cli.Options;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;

/**
 * Start worker decommission process.
 */
public class StartDecommissionCommand extends AbstractFsAdminCommand {
  private static final Logger LOG = LoggerFactory.getLogger(StartDecommissionCommand.class);

  private static final String HELP_OPTION_NAME = "h";
  private static final Option HELP_OPTION =
      Option.builder(HELP_OPTION_NAME)
          .required(false)
          .hasArg(false)
          .desc("print help information.")
          .build();
  private final AlluxioConfiguration mAlluxioConf;

  /**
   * @param context fsadmin command context
   * @param alluxioConf Alluxio configuration
   */
  public StartDecommissionCommand(Context context, AlluxioConfiguration alluxioConf) {
    super(context);
    mAlluxioConf = alluxioConf;
  }

  @Override
  public int run(CommandLine cl) throws IOException {
    if (cl.hasOption(HELP_OPTION_NAME)) {
      System.out.println(getUsage());
      System.out.println(getDescription());
      return 0;
    }
    FileSystemAdminShellUtils.checkMasterClientService(mAlluxioConf);
    try {
      mFsClient.decommissionWorkers();
    } catch (AlluxioStatusException e) {
      LOG.error(e.getMessage(), e);
      return -1;
    }
    return 0;
  }

  @Override
  public Options getOptions() {
    return new Options().addOption(HELP_OPTION);
  }

  @Override
  public String getCommandName() {
    return "start";
  }

  @Override
  public String getUsage() {
    return "start [-h]";
  }

  @Override
  public String getDescription() {
    return "Decommission workers from file defined by excluded-workers. If there are entries "
        + "in excluded-workers, the hosts in it are need to be decommissioned. Decommissioned "
        + "Worker are not automatically shutdown and are not chosen for writing new replicas.";
  }
}
