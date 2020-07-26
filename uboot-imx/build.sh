#!/bin/bash
#make mx6qsabresd_config
echo =============================step1==========================================
make mx6qsabresdandroid_config
echo 
echo 
echo 
echo 
echo 
echo 
echo =============================step2==========================================
#make V=1 -w -j8 
make  -j1 
echo =============================all done==========================================

