## Implementing two IP lookup strategies
### General requirement:
Input:
|input |routing table | trace file| 
|------|------|-------|
|entries|10^6|1.5x10^6|
|act as| prefix and rules | actual dump traffic|

Output: print the statisic of packets outgoing to different ports to the terminal.
Each entry of routing table is like: EX. 0.1.208.36/31 5 meaning: the prefix is 0.1.208.36, with prefix length 31, goes to output port 5.
