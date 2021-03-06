#!/bin/bash
set -e

tabnum=$1

# Navigate into this directory
cd "$( dirname "${BASH_SOURCE[0]}" )"

echo "RUNNING TABLE ${tabnum}" && bash table${tabnum}_main.sh

# Check their results
bash check.sh expected_table${tabnum}_main.csv table${tabnum}_main.csv
