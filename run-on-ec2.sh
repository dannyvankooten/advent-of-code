#!/bin/bash
set -e 
AWS="54.244.68.210"
rsync -az  -e "ssh -i ~/Downloads/pair1.pem" --exclude=".git/*" . ec2-user@$AWS:~/advent-of-code 
ssh -i ~/Downloads/pair1.pem ec2-user@$AWS 'cd ~/advent-of-code && make release && ./aoc2020'