/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>

#include "dictionary.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define LENGTH 45

typedef struct node
{
    bool itIsWord;
    struct node* children[27];
} node;

node* position = NULL;
node* root;

int counter = 0;
bool freeNodes(node* freeNode, int possition);
int getIndex(char letter){
    if (isalpha(letter)) 
		{
		    int l;
			if (isupper(letter)) 
			{
				l = (letter - 65);
			}
			if (islower(letter)) 
			{
				l = (letter - 97);
			}
			return l;
		} 
	else{
	    return 27;
	} 
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word){
    bool result = false;
    int i = 0;
    position = root;
    int letter = getIndex(word[i]);
    while(word[i] != '\0'){
        if(position->children[letter] != NULL){
            position = position->children[letter];
            if((word[i+1] == '\0') && (position->itIsWord == true)){
                result = true;
                goto stop;
            }
            else{
                result = false;
                }
            }
        else{
            result = false;
            goto stop;
        }
        i++;
        letter = getIndex(word[i]);
    }
    stop: 
    return result;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    char line[LENGTH + 1];
    node* newNode = NULL;
    FILE* infile = fopen(dictionary, "r");
    if (infile == NULL){
        return false;
    }
    
    while(fscanf(infile, "%s", line) != EOF){      

        int i = 0;
        position = root;
        int letter  = getIndex(line[i]);
        //перший раз створення
        if (root == NULL){
            root = malloc(sizeof(node));
            for(int k = 0; k < 27; k++){
                root->children[k] = NULL;
            }
            position = root;
        }

        while (line[i] != 0){            
            if (position->children[letter] == NULL){
                newNode = malloc(sizeof(node));
                for(int k = 0; k < 27; k++){
                    newNode->children[k] = NULL;
                }           
                if (line[i + 1] == '\0'){
                    newNode->itIsWord = true;
                    counter++;
                }
                else{
                    newNode->itIsWord = false;                   
                }
                position->children[letter] = newNode;
                position = position->children[letter];
            }
            else{
                position = position->children[letter];
            }
            i++;
            letter = getIndex(line[i]);
         }
    }
    fclose(infile);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    int sizeVoc = counter;
    return sizeVoc;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */

bool unload(void)
{
    // call freenode and pass the root of the trie and
    // start with the last position.
    return freeNodes(root, 26);
}


/**
 * Unloads a node from memory starting at the given position.  
 * Returns true if successful else false.
 */
bool freeNodes(node* freeNode, int possition)
{

    // check if the last position has not been reached
    if (possition >= 0)
    {
        // if the child node is not null        
        if (freeNode->children[possition] != NULL)
        {
            // call freenode recursively to free the child node starting 
            // at the last position
            freeNodes(freeNode->children[possition], 26);
            
            // free the child node
            free(freeNode->children[possition]);
        }
       
        // if parent node is not null        
        if (freeNode != NULL)
        {
            // call freenode recursively to free node starting 
            // one position (letter) before the current postion
            freeNodes(freeNode, possition - 1);
        }
     }

    // if the free node is the same as root and all postions have been
    // checked, free the root node
    if (freeNode == root && possition == 0)
    {
        free(root);
    }
    
    // that's all folks
    return true;
}    

