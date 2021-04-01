#!/bin/bash
set -e 

AWS="54.245.155.98"
rsync -az  -e "ssh -i ~/Downloads/pair1.pem" --exclude=".git/*" . ec2-user@$AWS:~/advent-of-code 

ssh -i ~/Downloads/pair1.pem ec2-user@$AWS 'cd ~/advent-of-code && ./runall.sh'