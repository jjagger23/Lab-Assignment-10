/*Programmer: Joshua Jaggernauth
Date: 04/19/2024
Project: Lab Assignment 10 (Trie Data Structure)
*/
//This C program creates a trie data structure to store a dictionary of words. It defines structures and functions to 
//handle trie nodes, insertion, retrieval, and memory deallocation. The main function reads words from a file, constructs 
//the trie, tests it with sample words, and then deallocates memory. Overall, the program efficiently manages and 
//organizes words using the trie data structure. At the end of the execution, the code will display the number of
//occurrences of each sample word in the trie. 

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#define ALPHABET_SIZE 26

//Trie Node structure
struct TrieNode {
  struct TrieNode * children[ALPHABET_SIZE];
  int isEndOfWord; //Flag indicating end of a word
  int count; //Number of occurrences of the word
};

//Initializes a TrieNode
struct TrieNode * createNode() {
  struct TrieNode * node = (struct TrieNode * ) malloc(sizeof(struct TrieNode));
  if (node) {
    node -> isEndOfWord = 0;
    node -> count = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
      node -> children[i] = NULL;
    }
  }
  return node;
}

//Inserts the word into the Trie
void insert(struct TrieNode * root,
  const char * word) {
  struct TrieNode * current = root;
  for (int i = 0; word[i] != '\0'; i++) {
    int index = word[i] - 'a';
    if (!current -> children[index]) {
      current -> children[index] = createNode();
    }
    current = current -> children[index];
  }
  current -> isEndOfWord = 1;
  current -> count++;
}

//Computes the number of occurrences of the word
int numberOfOccurrences(struct TrieNode * root,
  const char * word) {
  struct TrieNode * current = root;
  for (int i = 0; word[i] != '\0'; i++) {
    int index = word[i] - 'a';
    if (!current -> children[index]) {
      return 0; //Word not found
    }
    current = current -> children[index];
  }
  return (current && current -> isEndOfWord) ? current -> count : 0;
}

//Deallocates the Trie structure
void deallocateTrie(struct TrieNode * root) {
  if (!root)
    return;
  for (int i = 0; i < ALPHABET_SIZE; i++) {
    deallocateTrie(root -> children[i]);
  }
  free(root);
}

//Reads the dictionary from a file and stores the words
int readDictionary(char * filename, char ** pInWords) {
  FILE * file = fopen(filename, "r");
  if (!file) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  int numWords = 0;
  char word[100]; //Assuming maximum word length is 100

  while (fscanf(file, "%s", word) != EOF) {
    pInWords[numWords] = strdup(word);
    numWords++;
  }

  fclose(file);
  return numWords;
}

int main(void) {
  char ** inWords = malloc(256 * sizeof(char * ));
  if (!inWords) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }

  //Read the number of words in the dictionary
  int numWords = readDictionary("dictionary.txt", inWords);
  printf("Number of words read from dictionary: %d\n", numWords);

  struct TrieNode * root = createNode();
  for (int i = 0; i < numWords; i++) {
    insert(root, inWords[i]);
  }

  //Test with sample words
  char * pWords[] = {
    "notaword",
    "ucf",
    "no",
    "note",
    "corg"
  };
  for (int i = 0; i < 5; i++) {
    printf("\t%s : %d\n", pWords[i], numberOfOccurrences(root, pWords[i]));
  }

  //Deallocate Trie
  deallocateTrie(root);

  //Free dynamically allocated memory
  for (int i = 0; i < numWords; ++i) {
    free(inWords[i]);
  }
  free(inWords);

  return 0;
}