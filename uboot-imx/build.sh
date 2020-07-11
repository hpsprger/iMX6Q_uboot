#!/bin/bash
#make mx6qsabresd_config
echo step1==========================================
make mx6qsabresdandroid_config
echo step2==========================================
#make V=1 -w -j8 
make  -j8 
echo all done==========================================

