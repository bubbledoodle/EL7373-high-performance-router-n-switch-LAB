

```
E:
cd oracle/virtualbox
VBoxManage.exe dhcpserver add --netname intnet --ip 192.168.0.100 --netmask 255.255.255.0 --lowerip 192.168.1.100 --upperip 192.168.1.200 --enable
```

sys proc has some nice interface to control the network parameters. To change them you may use these command as direct change may result in under privillaged denying.
```
sudo bash -c "....."
sudo sysctl -w net.ipv4.ip_forward=1
```
Add the default gateway that all packet from inner network "intnet" send pkt from default gateway 192.168.1.101.
```
sudo route add default gateway 192.168.2.101
sudo ifdown enp0s3
sudo ifup enp0s3
```
From above, the FW has the forwarding enabled. 
