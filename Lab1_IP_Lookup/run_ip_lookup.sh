#!/bin/bash
rm *.out
g++ disjoint_prefix_lookup.c -o iplookup.out -lpcap
./iplookup.out trace.dump routing_table.txt

g++ prefix_range_lookup.c -o Rangebased_lookup.out -lpcap
./Rangebased_lookup.out trace.dump routing_table.txt
