#!/bin/bash
set -e 
AWS="54.186.28.90"
rsync -az  -e "ssh -i ~/Downloads/pair1.pem" --exclude=".git/*" . ec2-user@$AWS:~/advent-of-code 
ssh -i ~/Downloads/pair1.pem ec2-user@$AWS 'cd ~/advent-of-code && ./pgo.sh'
ssh -i ~/Downloads/pair1.pem ec2-user@$AWS 'cd ~/advent-of-code && ./aoc2020 --bench'
