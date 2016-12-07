## Implementing two IP lookup strategies
### General requirement:
Input:

Input | routing table | trace file 
--- | :---: | ---
entries | 10^6 | 1.5x10^6
act as | prefix and rules | actual dump traffic

Output: print the statisic of packets outgoing to different ports to the terminal.
Each entry of routing table is like: 
>EX. 0.1.208.36/31 5 meaning: the prefix is 0.1.208.36, with prefix length 31, goes to output port 5.

Now to run the experiement, in a terminal simplly type in:
```
./run_ip_lookup.sh
```
It will compile the c++ file and read packets trace. 
Now this version of code is stuck somewhere of prefix range algorithm. 
The basic idea for that is to:

1. first convert each prefix into two structs. One represent starting point of range, the other, end point.
2. second build a vector using binary search
3. modify each of end point's GreaterTo part to be next nearest point's EqualTo.
4. do IP look up.

I am afraid I have to leave this here and come back later, Got some EMERGENCY to work on.
