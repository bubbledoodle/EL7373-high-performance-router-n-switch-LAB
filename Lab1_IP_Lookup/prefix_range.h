#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <arpa/inet.h>
#include <vector>
#include <bitset>

using namespace std;

//struct Ranged_prefix represent each prefix range's number and its two pointer. (= P1, > P1) more detail refer to slice 03b.12
struct Ranged_prefix{
	uint32_t ranged_num;	
	int EqualTo;
	int GreaterTo;

}range_point;


void Convert_to_Range(uint32_t prefix, int prelen, int port_num, range_point *Start, range_point *End){
	uint32_t end_mask = 0xFFFFFFFF;
	for(i = 0; i < prelen; i++){
		end_mask = end_mask >> 1;
	}
	uint32_t start_mask = ~end_mask;
	Start->ranged_num = prefix && start_mask;
	Start->EqualTo = port_num;		Start->GreaterTo = port_num;
	End->ranged_num = prefix || end_mask;
	End->EqualTo = port_num;		End->GreaterTo = -1;
}


void insert_rules(vector<range_point> *vec, uint32_t prefix, int prelen, int portnum){
    static int     n_rules = 0;

#ifdef DEBUG
    uint32_t prefix_r = htonl(prefix);
    fprintf(stderr, "Insert rule: %-15s(%08x)/%d    %d\n", 
            inet_ntoa(*(struct in_addr *)&prefix_r), 
            prefix, prelen, portnum);
#endif

    n_rules ++;

    range_point *range_start;
    range_point *range_end;
    Convert_to_Range(prefix, prelen, portnum, *range_start, *range_end);

    // insert starting point
    int lb = -1;
    int up = vec.size();
    int mid;
    while(lb + 1 < up){
    	mid = lb + (up -lb) / 2;
    	if(list[mid]->ranged_num > range_start->ranged_num){
    		up = mid;
    	} else {
    		lb = mid;
    	}
    	vec.insert(vec.begin() + lb + 1, *range_start);
    }
    
    // inserting end point
    lb = -1;
    up = vec.size();
    while(lb + 1 < up){
    	mid = lb + (up -lb) / 2;
    	if(list[mid]->ranged_num > range_start->ranged_num){
    		up = mid;
    	} else {
    		lb = mid;
    	}
        vec.insert(vec.begin() + lb + 1, *range_end);
    }
}


void refine_rule(vector<range_point> *vec){
    for(i = 0; i < vec.size() - 1; i++){
        vec[i]->GreaterTo = vec[i + 1]->EqualTo;
    }
    vec.[vec.end()]->GreaterTo = -1;
}


int ip_lookup(vector<range_point> *vec, uint32_t ip){
    int lb = -1;
    int vec.size();
    int mid;
    while(lb + 1 < up){
        mid = lb + (up -lb) / 2;
        if(list[mid]->ranged_num > ip){
            up = mid;
        } else {
            lb = mid;
        }
        return vec[lb + 1]->GreaterTo;
    }
}