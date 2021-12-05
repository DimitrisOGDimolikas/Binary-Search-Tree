// BinarySearchTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>

using namespace std;

//structure to store the data of the BST
struct node {
    int data; //data of BST node
    node* left; //left node of the BST
    node* right; //right node of the BST
    bool deleted; //is node is deleted or not

    //constructor to auto initialize the node
    node() {
        left = NULL; //initialize left node to NULL
        right = NULL; //initialize right node to NULL
        deleted = false; //initialize delete to false as it is not delted by default
    }
};

//class for the BST
class BST {
private:
    node* root; //private data member for the root of the BST

public:
    //default constructor to initialize the call
    BST() {
        root = NULL; //init witht the NULL
    }

    //driver function for the insert function
    void insert(int num) {
        insert(&this->root, num); //calling recursion overridden function
    }

    //driver function for the search function
    bool search(int num) {
        return search(this->root, num); //calling recursion overridden function
    }

    //driver function for the delete function
    bool deleteNode(int num) {
        return deleteNode(&this->root, num); //calling recursion overridden function
    }

    //driver function for the min function
    int min() {
        return min(this->root); //calling recursion overridden function
    }

    //driver function for the max function
    int max() {
        return max(this->root); //calling recursion overridden function
    }

    //driver function for the print function
    void print() {
        int treeDepth = depth(root); //get the max tree depth
        int digits = countTotalDigits(root); //get the total number of digits in all nodes to make the matrix e.g. nodes containing 3,5,15 gives 4.

        char ** printMatrix = new char*[treeDepth]; //creating empty print matrix rows according to tree depth
        for (int i = 0;i < treeDepth;i++) { //loop over all the tree depth
            printMatrix[i] = new char[digits]; //create the columns according to digits
            for (int j = 0;j < digits;j++) { //loop over all columns and init with the 'x' sign
                printMatrix[i][j] = 'X';
            }
        }
        int x = 0; //init col to 0
        print(root, printMatrix, 0, x); //calling print recursion function to fill the matrix

        //print the entire matrix
        for (int i = 0;i < treeDepth;i++) {
            for (int j = 0; j < digits; j++) {
                cout << printMatrix[i][j];
            }
            cout << endl;
        }
    }

    //driver function for the print function
    void printSpace(int k1, int k2) { 
        printSpace(root,k1, k2); //calling overidden recursion functions
        cout << "\b "; //removing last extra ',' from the output
    }
    
private:
    //inserting element using recursion
    void insert(node** root, int num) {
        if (*root == NULL) { //if root is null then it means we need to insert here
            *root = new node; //create new node
            (*root)->data = num; //enter the number to the data
            return; //stop the recursion function
        }

        if (num < (*root)->data) { //if the num is less then the current root data
            insert(&(*root)->left, num); //go to the left node
        }
        else {
            insert(&(*root)->right, num); //else go to the right node of the same node
        }

        //checkin if reconstruction required
        int lefnode = countNode((*root)->left); //get number of nodes of left sub tree
        int rightnode = countNode((*root)->right); //get number of nodes of right sub tree

        if (lefnode > 2 * rightnode && rightnode > 0) { //check if the left is greater then the double number of nodes of the right subtree
            (*root)->left = reconstruct(&(*root)->left); //reconstruct the tree from the same node
        }
        else if(lefnode*2 < rightnode && lefnode > 0){
            (*root)->right = reconstruct(&(*root)->right); //else reconstruct the right subtree
        }
    }

    //searching element using the recursion
    bool search(node* root, int num) {
        if (root == NULL) { //if root is null then it means we transverse all the tree and node did not found
            return false;
        }

        if (root->data == num) { //if the data is found
            if (root->deleted) { //if the data is marked deleted, return false as it is not found
                return false;
            }
            return true; //return true if it is not marked as delted
        }

        if (num < root->data) { //if number is less then the current node data
            search(root->left, num); //move to the left node
        }
        else {
            search(root->right, num); //else move to right node
        }
    }

    //recursion function to delte the node
    bool deleteNode(node** root, int num) {
        if (*root == NULL) { //if the node is null it means that the node to be delted is not found
            return false;
        }
        
        if ((*root)->data == num) { //if the node is found
            if ((*root)->left != NULL && (*root)->right != NULL) { //if the node have both child
                (*root)->deleted = true; //mark the node as delted
            }
            else if ((*root)->left != NULL) { //if only left node is found
                node* temp = *root; //save the node to the temp
                *root = (*root)->left; //replace the node with the node's left member
                delete temp; //delete the temp
            }
            else if ((*root)->right != NULL) { //if only right node is found
                node* temp = *root; //save the node to the temp
                *root = (*root)->right; //replace the node with the node's right member
                delete temp;//delete the temp
            }
            else {
                delete (*root); //if the node has no childs delete the node
                *root = NULL; //set it as the NULL
            }

            return true; //return true
        }

        if (num < (*root)->data) { 
            return deleteNode(&(*root)->left, num);
        }
        else {
            return deleteNode(&(*root)->right, num);
        }

        if (overhelmedDelete(*root)) { //checl if the node is overhelmed by the deleted nodes if yes
            *root = reconstruct(root); //reconstruct the tree
        }
    }

    //tansverse the tree to get the sorted array from BST and delete its nodes (Helping Function)
    void getSorted(node ** root, vector<int>* arr) {

        if ((*root)->left != NULL) { //if node's left is not null
            getSorted(&(*root)->left, arr); //do recursion to the left
        }

        if (!(*root)->deleted) { //if node is not marked as delted
            (*arr).push_back((*root)->data); //add the data to the vector
            
        }

        if ((*root)->right != NULL) { //if node's right is not null
            getSorted(&(*root)->right, arr); //do recursion to the right
            delete* root; //delete the current node
            *root = NULL; //set current node to NULL
        }
        else {
            delete* root; //else delte current node
            *root = NULL; //set it to NULL
        }
    }

    //check if the nodes that marke as delted exceeded 50% of the total nodes (Helping Driver Function)
    bool overhelmedDelete(node* root) {
        int deleted = 0, notdelted = 0; 
        overhelmedDelete(root, deleted, notdelted); //call the recursion function that returns delted and not deleted nodes numbers

        if (deleted / (float)(notdelted+deleted) > 0.5) { //if the delted exceeded the total number of nodes
            return true; //return true
        }

        return false; //return false
    }

    //recursion function to get the delted numbers
    void overhelmedDelete(node* root, int &deleted, int &notdeleted) {
        if (root == NULL) { //if node is null
            return;
        }

        if (root->deleted) { //if node is marked as delted count to the deleted variable
            deleted++;
        }
        else {
            notdeleted++; //else count to the not delted varaible
        }
        overhelmedDelete(root->left, deleted, notdeleted); //recurse to the left
        overhelmedDelete(root->right, deleted, notdeleted); //recurse to the right
    }

    //convert the array to the minimum possible BST (Helping Function)
    node* arrayToBST(int arr[], int start, int end) {
        if (start > end) { //if start become greater then end its means that we are done 
            return NULL; //return NULL
        }

        int mid = (start + end) / 2; //calculate the mid of the array
        node* root = new node; //create new node
        root->data = arr[mid]; //add the data to the node with the calculated mid of array
        
        root->left = arrayToBST(arr, start, mid - 1); //recurse with the first divided array and save the resulted node to the left side of the current node
        root->right = arrayToBST(arr, mid + 1, end); //recurse with the second divided array and save the resulted node to the left side of the current node

        return root; //return the root that is just created
    }

    //reconstruction of the tree
    node* reconstruct(node** root) {
        vector<int> arr; //create an empty vector
        getSorted(root, &arr); //get the sorted array from the BST

        *root = arrayToBST(&arr[0], 0, arr.size() - 1); //create a new tree from the sorted array

        return *root; //return new root node of the created tree
    }

    //counting the nodes in the tree (Helping Function)
    int countNode(node* root) {
        
        if (root == NULL) { //if the node is NULL
            return 0;
        }
        int count = countNode(root->left) + countNode(root->right); //recursion to the left and right and calculate the sum so we get the some of each nodes

        if (!root->deleted) { //if the node is not marked as deleted
            count++; //count the node
        }

        return count; //return the count of the node
    }

    //recursion function to get the minimum of the BST
    int min(node* root) {
        if (root->left == NULL) { //if the node's left is NULL then it means that we are at the minimum node
            return root->data; //return the node data
        }

        return min(root->left); //recurse to the left side of the BST as left contain the minimum
    }

    //recursion function to get the minimum of the BST
    int max(node* root) {
        if (root->right == NULL) { //if the node's right is NULL then it means that we are at the maximum node
            return root->data; //return the node data
        }

        return max(root->right); //recurse to the right side of the BST as right contain the maximum
    }

    //calculating the depth of the function (Helping Function)
    int depth(node* root) {
        if (root == NULL) {
            return 0;
        }

        int leftD = depth(root->left); //recurse to the left of the node
        int rightD = depth(root->right); //recurse to the right of the node

        return leftD > rightD ? leftD + 1 : rightD + 1; //if left depth is greate add one to it and return else add 1 to the right depth and return.
    }
    
    //count the lenght of total digits for each node
    int countTotalDigits(node* root) {
        if (root == NULL) { 
            return 0;
        }

        int nDigit = to_string(root->data).length(); //get the length of the digits in integer

        return nDigit + countTotalDigits(root->left) + countTotalDigits(root->right); //add the count with the count of left and right subtree's digits
    }

    //recursion print function
    void print(node* root, char** arr, int row, int &col) {
        if (root == NULL) {
            return;
        }

        print(root->left, arr, row+1, col); //recurse by adding 1 to row that represent the depth and columns
        string data = to_string(root->data); //convert data to string

        for (int i = 0;i < data.length();i++, col++) { //loop over the data
            arr[row][col] = data[i]; //add the data of ith index to the matrix
        }

        print(root->right, arr, row+1, col); //recurse by adding 1 to row that represent the depth and columns
    }

    //print the numbers between k1 and k2
    void printSpace(node* root, int k1, int k2) {
        if (root == NULL) {
            return;
        }

        if (root->data >= k1 && root->data <= k2) { //if the data lies between k1 and k2
            cout << root->data<<","; //print the numbers to the screen
        }
        printSpace(root->left, k1, k2); //recurse to the left
        printSpace(root->right, k1, k2); //recurse to the right
    }
};


int main()
{
    BST bst;
    char ch = '~';

    do {
        cout << "|====================[Menu]====================|" << endl;
        cout << "|     Please select from following             |" << endl;
        cout << "|                                              |" << endl;
        cout << "|     1) Insert                                |" << endl;
        cout << "|     2) Delete                                |" << endl;
        cout << "|     3) Search                                |" << endl;
        cout << "|     4) Min                                   |" << endl;
        cout << "|     5) Max                                   |" << endl;
        cout << "|     6) Print Space                           |" << endl;
        cout << "|     7) Print                                 |" << endl;
        cout << "|     0) Exit                                  |" << endl;
        cout << "|                                              |" << endl;
        cout << "|==============================================|" << endl;
        cout << "Choice>>";
        cin >> ch;
        if (ch == '1') {
            cout << "Please keep inserting positive number, insert negative to return to main menu!" << endl;
            int num = 0;
            cin >> num;
            while (num >= 0) {
                bst.insert(num);
                cout << num << " is added in BST!" << endl;
                cin >> num;
            }
        }
        else if (ch == '2') {
            int num = 0;
            cout << "Please enter number to delete: ";
            cin >> num;
            if (bst.deleteNode(num)) {
                cout << num << " deleted sucessfully!" << endl;
            }
            else {
                cout << num << " not found!" << endl;
            }
        }
        else if (ch == '3') {
            int num = 0;
            cout << "Please enter number to search: ";
            cin >> num;
            if (bst.search(num)) {
                cout << num << " is found!" << endl;
            }
            else {
                cout << num << " not found!" << endl;
            }
        }
        else if (ch == '4') {
            cout << "The minimum number in BST: " << bst.min()<<endl;
        }
        else if (ch == '5') {
            cout << "The maximum number in BST: " << bst.max() << endl;
        }
        else if (ch == '6') {
            int k1, k2;
            cout << "Please enter number for starting space: ";
            cin >> k1;
            cout << "Please enter number for ending space: ";
            cin >> k2;

            cout << "The numbers between " << k1 << " and " << k2 << " space: ";
            bst.printSpace(k1, k2);
            cout << endl;
        }
        else if (ch == '7') {
            bst.print();
        }
    } while (ch != '0');
}