#!/bin/bash
executables=$1

echo "Executing $1"
$executables 2
echo ""
$executables 4
echo ""
$executables 8
echo ""
$executables 16
echo ""
$executables 32
echo ""
$executables 64
echo ""
$executables 128