#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <arpa/inet.h>
#include <vector>
#include <bitset>

using namespace std;
//#define DEBUG
//struct Ranged_prefix represent each prefix range's number and its two pointer. (= P1, > P1) more detail refer to slice 03b.12
typedef struct Ranged_prefix{
	uint32_t ranged_num;	
	int EqualTo;
	int GreaterTo;
    int start_flag;

}range_point;


void Convert_to_Range(uint32_t prefix, int prelen, int port_num, range_point *Start, range_point *End){
	uint32_t end_mask = 0xFFFFFFFF;
	for(int i = 0; i < prelen; i++){
		end_mask = end_mask >> 1;
	}
	uint32_t start_mask = ~end_mask;
	Start->ranged_num = prefix & start_mask;
	Start->EqualTo = port_num;		Start->GreaterTo = port_num;
    Start->start_flag = 1;
	End->ranged_num = prefix | end_mask;
	End->EqualTo = port_num;		End->GreaterTo = -1;
    End->start_flag = 0;
    //printf("need to insert: %08x %08x %d\n",end_mask, Start->ranged_num, Start->GreaterTo);

}


void insert_rules(vector<range_point> *vec, uint32_t prefix, int prelen, int portnum){
    static int     n_rules = 0;

#ifdef DEBUG
    uint32_t prefix_r = htonl(prefix);
    fprintf(stderr, "Insert rule: %-15s(%08x)/%d    %d  %d\n", 
            inet_ntoa(*(struct in_addr *)&prefix_r), 
            prefix, prelen, portnum, n_rules);
#endif

    n_rules ++;
    range_point range_start;
    range_point range_end;
    Convert_to_Range(prefix, prelen, portnum, &range_start, &range_end);
#ifdef CHECK_RANGE_POINT
    printf("need to insert: %08x %d %d\n",range_start.ranged_num, range_start.GreaterTo,range_start.EqualTo);
    printf("need to insert: %08x %d %d\n",range_end.ranged_num, range_end.GreaterTo, range_end.EqualTo);
#endif

    // insert starting point
    int lb = -1;
    int up = vec->size();
    int mid;
    while(lb + 1 < up){
    	mid = lb + (up - lb) / 2;
    	if(vec->at(mid).ranged_num < range_start.ranged_num){
    		lb = mid;
    	} else {
    		up = mid;
    	}
    }
    vec->insert(vec->begin() + lb + 1, range_start);
    //printf("veclen: %d\n",vec->size());

    // inserting end point
    lb = -1;
    up = vec->size();
    while(lb + 1 < up){
    	mid = lb + (up -lb) / 2;
    	if(vec->at(mid).ranged_num < range_end.ranged_num){
    		lb = mid;
    	} else {
    		up = mid;
    	}
    }
    vec->insert(vec->begin() + lb + 1, range_end);

}


void refine_rule(vector<range_point>* vec){
    for(int i = 0; i < vec->size() - 1; i++){
        if (vec->at(i).start_flag){
            vec->at(i).GreaterTo = vec->at(i + 1).EqualTo;
            //printf("now: %08x %d %d\n",vec->at(i).ranged_num, vec->at(i).GreaterTo,vec->at(i).EqualTo);
        }
    }
    vec->end()->GreaterTo = -1;
}


int ip_lookup(vector<range_point> *vec, uint32_t ip){
    int lb = -1;
    int up = vec->size();
    int mid;
    while(lb + 1 < up){
        mid = lb + (up -lb) / 2;
        if(vec->at(mid).ranged_num < ip){
            lb = mid;
        } else {
            up = mid;
        }
    }
    return vec->at(lb + 1).EqualTo;
}