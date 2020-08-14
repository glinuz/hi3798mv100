#!/bin/sh

./sample_product_encrypt_boot fastboot_sign.bin fastboot_enc.bin
./sample_product_crypto 0 1 bootargs_sign.bin bootargs_enc.bin
./sample_product_crypto 0 1 stbinfo_sign.bin stbinfo_enc.bin
./sample_product_crypto 0 1 apploader_sign.bin apploader_enc.bin
./sample_product_crypto 0 1 base.img base_enc.img
./sample_product_crypto 0 1 logo.img logo_enc.img
./sample_product_crypto 0 1 system_sign.bin system_enc.bin
./sample_product_crypto 0 2 hdcp_key.bin hdcp_key_enc.bin
chmod 777 *.bin

./sample_product_burn_partition system system_enc.bin
./sample_product_burn_partition baseparam base_enc.img
./sample_product_burn_partition logo logo_enc.img
./sample_product_burn_partition loader apploader_enc.bin
./sample_product_burn_partition stbid stbinfo_enc.bin
./sample_product_burn_partition bootargs bootargs_enc.bin
./sample_product_burn_partition boot fastboot_enc.bin
./sample_product_burn_partition privatedata hdcp_key_enc.bin

#./product_pv_stb 1 pv_cfg_dbg.txt pv_out_dbg.txt

chmod 777 pv_out_*.txt