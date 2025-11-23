# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "BathGuardian.bin"
  "BathGuardian.map"
  "app.css.S"
  "app.js.S"
  "aws_root_ca_pem.S"
  "bootloader\\bootloader.bin"
  "bootloader\\bootloader.elf"
  "bootloader\\bootloader.map"
  "certificate_pem_crt.S"
  "config\\sdkconfig.cmake"
  "config\\sdkconfig.h"
  "esp-idf\\esptool_py\\flasher_args.json.in"
  "esp-idf\\mbedtls\\x509_crt_bundle"
  "favicon.ico.S"
  "flash_app_args"
  "flash_bootloader_args"
  "flash_project_args"
  "flasher_args.json"
  "index.html.S"
  "jquery-3.3.1.min.js.S"
  "ldgen_libraries"
  "ldgen_libraries.in"
  "private_pem_key.S"
  "project_elf_src_esp32.c"
  "x509_crt_bundle.S"
  )
endif()
