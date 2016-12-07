#!/bin/bash
sudo ip rule add to 192.168.20.0/24 table 1
sudo ip route add 192.168.1.0/24 dev enp0s8 scope link table 1
sudo ip route add default via 192.168.1.99 dev enp0s8 table 1
