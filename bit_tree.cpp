#include<iostream>
#include<vector>
#include<cmath>
#include<cstring>
#include <algorithm>
#include <queue>

using namespace std;

struct Node
{
    string val_str;
    int val;
    Node* left;
    Node* right;
};

//function to create a string of n_bit zeroes
string init_binary_str(int n_bits)
{
    string temp = "";
    for(int i=0; i<n_bits; i++)
    {
        temp = temp + '0';
    }
    return temp;
}

//function to convert a number to binary
string to_binary(int num)
{
    string b_num = "";

    //if num is 0, return "0"  
    if(num == 0)    
    {
        return "0";
    }

    while(num > 0)
    {
        char c = (num % 2) + 48;     //converting the 0 or 1 to char
        b_num = b_num + c;
        num = num / 2;
    }

    reverse(b_num.begin(), b_num.end());    //reverse the string

    return b_num;
}


void generate_binary_nos(int n_bits, vector<string> &binary_numbers)
{
    int total_binary_numbers = binary_numbers.size();

    //loop through all the binary numbers 0 - 2^n_bits

    for(int i=0; i<total_binary_numbers; i++)        
    {
        string binary_num_str = init_binary_str(n_bits);  //initializes the binary_num_str to a string of zeroes of n-bits eg. 0000 for 4 bits

        //convert i to binary number in string
        string b_num = to_binary(i);

        //last index of the binary_num_str
        int l_main = n_bits - 1;     

        //last index of the b_num string
        int l_sec = b_num.length() - 1; 

        while(l_main >=0 && l_sec >=0 )
        {
            binary_num_str[l_main--] = b_num[l_sec--];
        }

        binary_numbers[i] = binary_num_str;
    }
}

//function to initialize the queue with the leaf nodes first
void init_node_queue(queue<Node *> &node_queue, vector<string> &binary_numbers)
{
    int n = binary_numbers.size();
    
    for(int i=0; i<n; i++)
    {
        Node* new_node = new Node;
        new_node->left = NULL;
        new_node->right = NULL;
        new_node->val = i;
        new_node->val_str = binary_numbers[i];
        node_queue.push(new_node);
    }
}

//helper function of create_tree function. This function finds the part (sub-strnig) of both strings that are the same.
string find_same(string &left_str, string &right_str)
{
    int n = left_str.size();
    string same_str = "";
    int i = 0;
    while(i < n && left_str[i] == right_str[i])
    {
        same_str = same_str + left_str[i];
        i++;
    }

    return same_str;
}

//helper function of create_tree function. This function finds the maximum of the left subtree
int find_max(Node* left)
{
    Node* temp = left;
    while(temp->right != NULL)
    {
        temp = temp->right;
    }
    return temp->val;
}

Node* create_tree(queue<Node *> &node_queue)
{
    while(node_queue.size() != 1)
    {
        Node* left = node_queue.front();
        string left_str = left->val_str;
        node_queue.pop();

        Node* right = node_queue.front();
        string right_str = right->val_str;
        node_queue.pop();

        Node* new_node = new Node;
        new_node->left = left;
        new_node->right = right;

        new_node->val_str = find_same(left_str, right_str);

        new_node->val = find_max(left);

        node_queue.push(new_node);
    }
    Node* root =  node_queue.front();
    root->val_str = "root";
    return root;
}

vector<Node* > find_path(Node* root, int n, int num)
{
    vector<Node* > path (n+1);
    Node* temp = root;
    
    int i=0;
    while(temp!=NULL && i<n+1)
    {
        path[i] = temp;
        if (num <= temp->val)
        {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
        i++;
    }

    return path;
}

//function that finds the v_split node
int find_divervent_node(vector<Node *> &path_low, vector<Node *> &path_high)
{
    int n = path_low.size();
    int i = 0;
    while (i < n && path_low[i]->val_str == path_high[i]->val_str)
    {
        i++;
    }

    if(i == n)
    {
        cout << "You inputted same numbers\n";
        return 0 ;
    }

    return i-1;   //position of the split
    
}

bool isleaf(Node* node)
{
    if(node->left == NULL && node->right == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

vector<Node *> find_range(Node *root, int n)
{
    int low = 0;
    int high = 0;
    cout << "Enter the interval for the range [low, high]" << "\n";
    cout << "low: ";
    cin >> low;
    cout << "high: ";
    cin >> high;

    vector<Node *> path_low(n + 1);    //vector to hold the path from root to low
    vector<Node *> path_high(n + 1);   //vector to hold the path from root to high

    path_low = find_path(root, n, low);
    path_high = find_path(root, n, high);

    int pos = find_divervent_node(path_low, path_high);   //position of v_split

    vector<Node *> range;

    //range of the left of v_split 
    for(int i = pos+1; i < n+1; i++)
    {
        if(i == n-1)   //last second node
        {
            if(path_low[i]->val == path_low[i+1]->val)
            {
                range.push_back(path_low[i]);
                break;
            }
        }
        
        if(path_low[i]->val >= low)
        {
            if(isleaf(path_low[i]))
            {
                range.push_back(path_low[i]);
            }
            else
            {
                range.push_back(path_low[i]->right);
            }
        }  
    }

    //range of the right of v_split
    for(int i = pos+1; i < n+1; i++)
    {
        if(i == n-1)  //last second node
        {
            if(path_high[i]->val < path_high[i+1]->val)
            {
                range.push_back(path_high[i]);
                break;
            }
        }

        if(path_high[i]->val < high)
        {
            range.push_back(path_high[i]->left);
        }

        if(isleaf(path_high[i]))
        {
            range.push_back(path_high[i]);
        }
    }
    
    return range;
}

void display_path(vector<Node *> path)
{
    int n = path.size();
    cout << "The path is: ";
    for(int i = 0; i < n ; i++)
    {
        cout << path[i]->val_str << " -> ";
    }
    cout << "\n";
}

void display_range(vector<Node *> range)
{
    int n = range.size();
    cout << "The Range is [ ";
    for(int i=0; i<n; i++)
    {
        cout << range[i]->val_str << ", ";
    }
    cout << "]" << "\n";
}

bool range_query(Node* root, int n)
{
    vector<Node* > range = find_range(root, n);

    int num;
    cout << "Enter the number which you want to query: ";
    cin >> num;

    vector<Node*> path (n+1);
    path = find_path(root, n, num);

    int range_size = range.size();

    for(int i=1; i<n+1; i++)
    {
        for(int j=0; j<range_size; j++)
        {
            if(path[i]->val_str == range[j]->val_str )
            {
                return true;
            }
        }
    }

    return false;
}

int main()
{
    int n_bits = 0;     // holds the required number of bits
    cout << "Enter how many bits do you want: ";
    cin >> n_bits;

    int total_binary_numbers = pow(2,n_bits);         //total number of binary numbers that can be created with n_bits


    //string vector to store the binary numbers
    vector<string> binary_numbers (total_binary_numbers);

    //generate all binary numbers that can be formed with n_bits and stores them in binary_numbers vector
    generate_binary_nos(n_bits, binary_numbers);

    queue<Node*> node_queue;
    
    init_node_queue(node_queue, binary_numbers);   //initialize the queue with the leaf nodes

    Node* root = create_tree(node_queue);

    int ans;
    cout << "Enter 1 to find the path of a number. " 
         << "\n"
         << "Enter 2 to find the range of an interval. "
         << "\n"
         << "Enter 3 to perform a range query on an interval"
         << "\n";
    cin >> ans;
    if(ans == 1)
    {
        int num;
        cout << "Enter the number for which you want to find the path:";
        cin >> num;
        vector<Node *> path(n_bits + 1);
        path = find_path(root, n_bits, num);
        display_path(path);
    }
    else if(ans == 2)
    {
        vector<Node *> range;
        range = find_range(root, n_bits);
        display_range(range);
    }
    
    else if(ans == 3)
    {
        bool query = range_query(root, n_bits);
        if (query)
        {
            cout << "The number is in the range."
                 << "\n";
        }
        else
        {
            cout << "The number is NOT in the range."
                 << "\n";
        }
    }

}