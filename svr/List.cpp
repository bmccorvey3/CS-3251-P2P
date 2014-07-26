/**
 * CS 2110 - Summer 2012 - Homework #10
 * Edited by: Brandon Whitehead, Andrew Wilder, Dan Fitzgerald
 *
 * list.c: Complete the functions!
 */

#include <stdlib.h>
#include <stdio.h>
#include "List.h"

typedef struct lnode
{
  struct lnode* prev; /* Pointer to previous node */
  struct lnode* next; /* Pointer to next node */
  void* data; /* User data */
} node;


/* Do not create any global variables here. Your linked list library should obviously work for multiple linked lists */
// This function is declared as static since you should only be calling this inside this file.
static node* create_node(void* data);

/** create_node
  *
  * Helper function that creates a node by allocating memory for it on the heap.
  * Be sure to set its pointers to nullptr.
  *
  * @param data a void pointer to data the user wants to store in the list
  * @return a node
  */
static node* create_node(void* data)
{
    node* newNode = malloc(sizeof(node));
    if (newNode != nullptr)
    {
        newNode->data = data;
        newNode->next = nullptr;
        newNode->prev = nullptr;
    }
    return newNode;
}

/** create_list
  *
  * Creates a list by allocating memory for it on the heap.
  * Be sure to initialize size to zero and head/tail to nullptr.
  *
  * @return an empty linked list
  */
list* create_list(void)
{
    list* myList = (list*) malloc(sizeof(list));
    if (myList != nullptr) // if we successfully allocated memory
    { // initialize pointers
        myList->head = nullptr;
        myList->tail = nullptr;
        myList->size = 0;
    } // if it is nullptr, pass the nullptr on
    return myList;
}

/** push_front
  *
  * Adds the data to the front of the linked list
  *
  * @param llist a pointer to the list.
  * @param data pointer to data the user wants to store in the list.
  */
void push_front(list* llist, void* data)
{
    node* oldHead = llist->head; // get old head
    node* newNode = create_node(data); // make new node for data
    if (newNode != nullptr) // if successfully created new node
    {
        llist->head = newNode;
        if (llist->size != 0) // if there was an element in the list before
        { // set next/previous references
            newNode->next = oldHead;
            oldHead->prev = newNode;
        }
        else // if the list was previously empty
        { // this is also the tail
            llist->tail = newNode;
        }
        llist->size++;
    }

}

/** push_back
  *
  * Adds the data to the back/end of the linked list
  *
  * @param llist a pointer to the list.
  * @param data pointer to data the user wants to store in the list.
  */
void push_back(list* llist, void* data)
{
    node* oldTail = llist->tail;
    node* newNode = create_node(data);
    if (newNode != nullptr)
    {
        llist->tail = newNode;
        if (llist->size != 0) // if there was an element in the list before
        { // set next/previous references
            oldTail->next = newNode;
            newNode->prev = oldTail;
        }
        else // if the list was previously empty
        { // this is also the tail
            llist->head = newNode;
        }
        llist->size++;
    }
}

/** remove_front
  *
  * Removes the node at the front of the linked list
  *
  * @warning Note the data the node is pointing to is also freed. If you have any pointers to this node's data it will be freed!
  *
  * @param llist a pointer to the list.
  * @param free_func pointer to a function that is responsible for freeing the node's data.
  * @return -1 if the remove failed (which is only there are no elements) 0 if the remove succeeded.
  */
int remove_front(list* llist, list_op free_func)
{
    /// @note remember to also free the node itself
    /// @note free_func is a function that is responsible for freeing the node's data only.
    if (llist->head != nullptr)
    {
        node* oldHead = llist->head;
        free_func(oldHead->data); // free data
        llist->head = oldHead->next; // set front to the next node
        if (llist->head != nullptr)
        {
            llist->head->prev = nullptr;
        }
        free(oldHead); // free node
        llist->size--;
        return 0;
    }
    return -1;
}

/** remove_back
  *
  * Removes the node at the back of the linked list
  *
  * @warning Note the data the node is pointing to is also freed. If you have any pointers to this node's data it will be freed!
  *
  * @param llist a pointer to the list.
  * @param free_func pointer to a function that is responsible for freeing the node's data.
  * @return -1 if the remove failed 0 if the remove succeeded.
  */
int remove_back(list* llist, list_op free_func)
{
    /// @note Remember to also free the node itself
    /// @note free_func is a function that is responsible for freeing the node's data only.
    if (llist->tail != nullptr)
    {
        node* oldTail = llist->tail;
        free_func(oldTail->data);
        llist->tail = oldTail->prev;
        if (llist->tail != nullptr)
        {
            llist->tail->next = nullptr;
        }
        free(oldTail);
        llist->size--;
        return 0;
    }
    return -1;
}

/** copy_list
  *
  * Create a new list structure, new nodes, and new copies of the data by using
  * the copy function. Its implementation for any test structure must copy
  * EVERYTHING!
  *
  * @param llist A pointer to the linked list to make a copy of
  * @param copy_func A function pointer to a function that makes a copy of the
  *        data that's being used in this linked list, allocating space for
  *        every part of that data on the heap
  * @return The linked list created by copying the old one
  */
list* copy_list(list* llist, list_cpy copy_func)
{
	list* newList = create_list();
	if (newList != nullptr)
	{
        newList->size = llist->size; // copy size
        if (llist->size > 0) // if there are things to copy
        {
            node* curNode = llist->head; // get first node to copy
            node* oldCopyNode = create_node(copy_func(curNode->data)); // copy data and make new node
            newList->head = oldCopyNode; // set head
            node* newCopyNode = nullptr;
            curNode = curNode->next; // advance to next node
            while(curNode != nullptr)
            {
                newCopyNode = create_node(copy_func(curNode->data)); // copy data and make new node
                newCopyNode->prev = oldCopyNode; // set prev
                oldCopyNode->next = newCopyNode; // set next
                curNode = curNode->next; // advance to next node
                oldCopyNode = newCopyNode; // advance the destination copy
            }
            if (newCopyNode != nullptr) // if there was more than one item in the list
            { // we actually initialized the newCopyNode, so that's the last node
                newList->tail = newCopyNode;
            }
            else // if there's only one item in the list
            {
                newList->tail = oldCopyNode;
            }
        }
	}
	return newList;
}

/** front
  *
  * Gets the data at the front of the linked list
  * If the list is empty return nullptr.
  *
  * @param llist a pointer to the list
  * @return The data at the first node in the linked list or nullptr.
  */
void* front(list* llist)
{
    /// @note you are returning the HEAD's DATA not the head node. Remember the user should never deal with the linked list nodes.
    if (llist->head != nullptr)
    {
        return llist->head->data; // if the data is nullptr, return it anyway
    }
    return nullptr; // otherwise, return nullptr
}

/** back
  *
  * Gets the data at the "end" of the linked list
  * If the list is empty return nullptr.
  *
  * @param llist a pointer to the list
  * @return The data at the last node in the linked list or nullptr.
  */
void* back(list* llist)
{
    if (llist->tail != nullptr)
    {
        return llist->tail->data; // if the data is nullptr, return it anyway
    }
    return nullptr; // otherwise, return nullptr
}

/** size
  *
  * Gets the size of the linked list
  *
  * @param llist a pointer to the list
  * @return The size of the linked list
  */
int size(list* llist)
{
    return llist->size;
}

/** traverse
  *
  * Traverses the linked list calling a function on each node's data.
  *
  * @param llist a pointer to a linked list.
  * @param do_func a function that does something to each node's data.
  */
void traverse(list* llist, list_op do_func)
{
    if(llist->size != 0) // if there are nodes to traverse
	{
        node* curNode = llist->head;
        while (curNode != nullptr)
        {
            do_func(curNode->data); // do something to the data
            curNode = curNode->next; // advance to next node
        }
	}
}

/** remove_if
  *
  * Removes all nodes whose data when passed into the predicate function returns true
  *
  * @param llist a pointer to the list
  * @param pred_func a pointer to a function that when it returns true it will remove the element from the list and do nothing otherwise @see list_pred.
  * @param free_func a pointer to a function that is responsible for freeing the node's data
  * @return the number of nodes that were removed.
  */
int remove_if(list* llist, list_pred pred_func, list_op free_func)
{
    /// @note remember to also free all nodes you remove.
    /// @note be sure to call pred_func on the NODES DATA to check if the node needs to be removed.
    /// @note free_func is a function that is responsible for freeing the node's data only.
    // llist->size--;
    node* curNode = llist->head;
    node* prevNode = nullptr;
    node* nextNode = nullptr;
    int numNodesRem = 0;
    while (curNode != nullptr)
    {
        if (pred_func(curNode->data)) // if we want to remove the node (and its data)
        {
            // remove its reference from pointers around it
            nextNode = curNode->next;
            if (prevNode != nullptr) // if this is not head node
            {
                prevNode->next = nextNode; // set previous node to the next node
            }
            else // if this is the head, correct head reference
            {
                llist->head = nextNode;
            }
            if (nextNode != nullptr) // if this isn't the tail
            {
                nextNode->prev = prevNode;
            }
            else // if this is the tail, fix tail reference
            {
                llist->tail = prevNode;
            }
            // remove node and free data
            free_func(curNode->data); // free the data
            free(curNode); // free the node itself
            curNode = nextNode; // update next node (prevNode stays the same)
            numNodesRem++;
            llist->size--; // decrement size
        }
        else
        {
            // update references
            prevNode = curNode;
            curNode = curNode->next;
        }
    }
    return numNodesRem;
}

/** is_empty
  *
  * Checks to see if the list is empty.
  *
  * @param llist a pointer to the list
  * @return 1 if the list is indeed empty 0 otherwise.
  */
int is_empty(list* llist)
{
    ///@note an empty list by the way we want you to implement it has a size of zero and head points to nullptr.
    if (llist->size == 0) // if empty
        return 1; // it's empty
    return 0; // otherwise, it's not empty
}

/** empty_list
  *
  * Empties the list after this is called the list should be empty.
  *
  * @param llist a pointer to a linked list.
  * @param free_func function used to free the node's data.
  *
  */
void empty_list(list* llist, list_op free_func)
{

    /// @note Free all of the nodes not the linked list itself.
    /// @note do not free llist.
    if(llist->size != 0) // if there are nodes to traverse
	{
        node* curNode = llist->head;
        while (curNode != nullptr)
        {
            free_func(curNode->data); // free the data
            node* next = curNode->next; // get the reference to the next node before destroying this one
            free(curNode); // free the node itself
            curNode = next; // advance to next node
        }
        llist->head = nullptr;
        llist->tail = nullptr;
        llist->size = 0;
	}
}
