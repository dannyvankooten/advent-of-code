#!/bin/bash
set -e 

AWS="ec2-52-41-173-74.us-west-2.compute.amazonaws.com"
rsync -az  -e "ssh -i ~/Downloads/pair1.pem" --exclude=".git/*" . ec2-user@$AWS:~/ 

ssh -i ~/Downloads/pair1.pem ec2-user@$AWS 'cd ~/advent-of-code && ./runall.sh'