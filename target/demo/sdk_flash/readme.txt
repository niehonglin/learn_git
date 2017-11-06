Readme for adding HTML pages
============================

Requirements:
------------
Linux System

Steps:
-----
* Copy the HTML files which needed to be added, to the same directory where all the scripts are present.
* Create a file named "filelist" in the same directory
* Add the htmlfile names to the file "filelist" one by one with each filename in one line.
  Note: "filelist" should not contain any blank lines.
* Need to run flash_readonly_dset_update.sh with appropriate parameters. This script allows to stich html file dsets
  and tunable dsets together and create a read only dset image which gets appended with the binaries provided as input.
* Copy the necessary binaries (flashotp and raw_flash images) from BIN-HT\bin to the location of htmltoflash directory and 
  run the following command.

  sh flash_readonly_dset_update.sh <rawflash filename> <flashotp filename> <location of tunable dset binary> <name of readonly dset to be created>

  For example, if raw_flash filename is raw_flashimage_AR400X_REV4_IOT_SP144_hosted.bin, flashotp filename is flashotp_AR400X_REV4_IOT_SP144_hosted.bin,
  tunable dset directory is ../tunable_dset/sp144_4bitflash and readonly dset filename to be created is myrodset.bin. then the command to be executed is 
  as follows.

  sh flash_readonly_dset_update.sh raw_flashimage_AR400X_REV4_IOT_SP144_hosted.bin flashotp_AR400X_REV4_IOT_SP144_hosted.bin ../tunable_dset/sp144_4bitflash myrodset.bin.

  This command would generate raw_flashimage_AR400X_REV4_IOT_SP144_hosted.bin.new, flashotp_AR400X_REV4_IOT_SP144_hosted.bin.new and ota_raw_flashimage_AR400X_REV4_IOT_SP144_hosted.bin.new which contains the new image that has myrodset.bin attached to all of them.

Note:
-----

  raw_flashimage_AR400X_REV4_IOT_SP144_hosted.bin.new can be used for programming the firmware image through JTAG or ART2
  flashotp_AR400X_REV4_IOT_SP144_hosted.bin.new can be used to program the firmware through host.exe utility
  ota_raw_flashimage_AR400X_REV4_IOT_SP144_hosted.bin.new is the image to be used as OTA binary from the server
