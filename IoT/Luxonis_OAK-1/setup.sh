#!/bin/bash
sudo apt update && sudo apt upgrade -y
sudo apt install python3-pip libatlas-base-dev -y
python3 -m pip install -r requirements.txt

