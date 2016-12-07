#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

//#define DEBUG

/* Structure of binary trie node */
struct BtNode{
    BtNode  *left;      /* for 0 */
    BtNode  *right;     /* for 1 */
    int     verdict;
};

/* Initialize binary trie node */
BtNode* init_btnode(){
    BtNode *ret = (BtNode *)malloc(sizeof(BtNode));
    ret->left = NULL;
    ret->right = NULL;
    ret->verdict = -1;
    return ret;
}

/* Clean up binary trie */
void free_bt(BtNode *root){

    if(root->left != NULL){
        free_bt(root->left);
    }
    if(root->right != NULL){
        free_bt(root->right);
    }

    free(root);
}

/* Insert a rule */
void insert_rule(BtNode *root, uint32_t prefix, int prelen, int portnum){
    static int     n_rules = 0;

#ifdef DEBUG
    uint32_t prefix_r = htonl(prefix);
    fprintf(stderr, "Insert rule: %-15s(%08x)/%d    %d\n", 
            inet_ntoa(*(struct in_addr *)&prefix_r), 
            prefix, prelen, portnum);
#endif

    n_rules ++;

    /* default rule: if packet matches none of the rules, 
     * it will match this default rule, i.e. 0.0.0.0/0 */
    if( prelen == 0 ){
        root->verdict = portnum;
        return;
    }

    uint32_t    temp_prefix = prefix;
    BtNode      *curr_node = root;
    for(int i=0 ; i<prelen ; i++){
        int     curr_bit = (temp_prefix & 0x80000000) ? 1 : 0;
        if(curr_bit == 0){
            if(curr_node->left == NULL){
                curr_node->left = init_btnode();
            }
            curr_node = curr_node->left;
        }
        else{
            if(curr_node->right == NULL){
                curr_node->right = init_btnode();
            }
            curr_node = curr_node->right;
        }
        temp_prefix = temp_prefix << 1;
    }

    if( curr_node->verdict != -1 ){
        fprintf(stderr, "Error: Rule #%d - overwriting a previous rule!! \n", n_rules);
    }
    curr_node->verdict = portnum;
}
void leaves_push(BtNode* bt_root){
    if(bt_root == NULL) return;
    if(bt_root->left != NULL || bt_root->right != NULL){
        if(bt_root->verdict != -1){
            if(bt_root->left == NULL){
                bt_root->left = init_btnode();
            }
            if(bt_root->right == NULL){
                bt_root->right = init_btnode();
            }
            if(bt_root->left->verdict == -1){
                bt_root->left->verdict = bt_root->verdict;
            }
            if(bt_root->right->verdict == -1){
                bt_root->right->verdict = bt_root->verdict;
            }
            bt_root->verdict = -1;
        }
        leaves_push(bt_root->left);
        leaves_push(bt_root->right);
    }
}
/*  1. first check root is NULL or not, really important
	2. no local variable needed, directly use root
	3. once prune out root NULL, logic all shift as you know. 
void leaves_push(BtNode *root){


    BtNode      *curr_node = root;
    if((curr_node->left == NULL) && (curr_node->right == NULL)){
    }
    else{
    	if(curr_node->verdict != -1){
    		int temp = curr_node->verdict;
    		curr_node->verdict = -1;
    		if((curr_node->left != NULL) && (curr_node->right == NULL)){
    			curr_node->right = init_btnode();
    			curr_node->right->verdict = temp;	
    		}
    		if((curr_node-left == NULL) && (curr_node->right != NULL)){
    			curr_node->left = init_btnode();
    			curr_node->left->verdict = temp;
    		}
    	}
    	else{
    		if((curr_node->left != NULL) && (curr_node->right == NULL)){
    			curr_node->right = init_btnode();
    			curr_node->right->verdict = temp;	
    		}
    		if((curr_node-left == NULL) && (curr_node->right != NULL)){
    			curr_node->left = init_btnode();
    			curr_node->left->verdict = temp;
    	}    	
    }

    if(root->left!= NULL){
		leaves_push(root->left);
    }

	if(root->right!=NULL){
		leaves_push(root->right);
	}
}
*/ //logic mess up
/* Look up an IP address (represented in a uint32_t) */
int lookup_ip(BtNode *root, uint32_t ip){
    uint32_t    temp_ip = ip;
    BtNode      *curr_node = root;
    int         curr_verdict = root->verdict;
    int         curr_bit = 0;

    while(1){
        curr_bit = (temp_ip & 0x80000000) ? 1 : 0;
        if(curr_bit == 0){
            if(curr_node->left == NULL)     return curr_verdict;
            else                            curr_node = curr_node->left;
        }
        else{
            if(curr_node->right == NULL)    return curr_verdict;
            else                            curr_node = curr_node->right;
        }

        /* update verdict if current node has an non-empty verdict */
        curr_verdict = (curr_node->verdict == -1) ? curr_verdict : curr_node->verdict;
        temp_ip = temp_ip << 1;
    }
}
