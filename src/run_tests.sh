#!/bin/bash


echo "./test_init"
./test_init
echo 
echo "./test_fusion"
./test_fusion $1
echo 
echo "./test_fusion_bzero"
./test_fusion_bzero $1
echo 
echo "./test_base"
./test_base $1
echo
echo "./test_base_bzero"
./test_base_bzero $1
echo 
echo "./test_frag"
./test_frag $1
echo "./test_cheese"
./test_cheese $1
echo
echo "./test_cheese_bzero"
./test_cheese_bzero $1
echo