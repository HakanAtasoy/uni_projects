#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>

#include "Node.h"

template<class T>
class LinkedList {
public: // DO NOT CHANGE THIS PART.
    LinkedList();
    LinkedList(const LinkedList<T> &obj);

    ~LinkedList();

    int getSize() const;
    bool isEmpty() const;
    bool contains(Node<T> *node) const;

    Node<T> *getFirstNode() const;
    Node<T> *getLastNode() const;
    Node<T> *getNode(const T &data) const;
    Node<T> *getNodeAtIndex(int index) const;

    void insertAtTheFront(const T &data);
    void insertAtTheEnd(const T &data);
    void insertAfterNode(const T &data, Node<T> *node);
    void insertAsEveryKthNode(const T &data, int k);

    void removeNode(Node<T> *node);
    void removeNode(const T &data);
    void removeAllNodes();
    void removeEveryKthNode(int k);

    void swap(Node<T> *node1, Node<T> *node2);
    void shuffle(int seed);

    void print(bool reverse=false) const;

    LinkedList<T> &operator=(const LinkedList<T> &rhs);

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.

private: // DO NOT CHANGE THIS PART.
    Node<T> *head;

    int size;
};

template<class T>
LinkedList<T>::LinkedList() {
    
    head = NULL ;
    size = 0 ;
}

template<class T>
LinkedList<T>::LinkedList(const LinkedList<T> &obj) {
    
    this -> size = 0 ;
    if (obj.size == 0)
    {
        this -> head = NULL ;
    }
    
    else
    {
        int s = obj.size ;
        Node <T> * frank = obj.head, *tmp = NULL , *hey = NULL ;
        this -> head = new Node<T>(frank -> data) ;
        tmp = head ;
        head -> next = head ;
        frank = frank -> next ;
        size++ ;
        for(int i = 1 ; i < s; i++)
        {
            hey = new Node<T>(frank -> data) ;
            hey -> prev = tmp ;
            tmp -> next = hey ;
            tmp = hey ;
            frank = frank -> next ;
            size++ ;
        }
        if (s != 1) hey -> next = head ;
        head -> prev = tmp ;
    }
    
}

template<class T>
LinkedList<T>::~LinkedList() {
    
    if (head == NULL) ;
    else 
    {
        Node <T> * destroyer = head, *tmp = NULL ;
        for(int i = 0; i < size; i++)
        {
            if (i != size - 1) tmp = destroyer -> next ;
            delete destroyer ;
            destroyer = tmp ;
        }
        head = NULL ;
        size = 0 ;
    }
}

template<class T>
int LinkedList<T>::getSize() const {
    
    return this -> size ;
}

template<class T>
bool LinkedList<T>::isEmpty() const {
    
    return this -> size == 0 ;
}

template<class T>
bool LinkedList<T>::contains(Node<T> *node) const {
    
    Node<T> * dog = head ;
    for(int i = 0; i < size; i++)
    {
        if((dog -> data == node -> data) && (dog -> prev == node -> prev) && (dog -> next == node -> next)) return true ;
        dog = dog -> next ;
    }
    return false ;
}

template<class T>
Node<T> *LinkedList<T>::getFirstNode() const {
    
    return head ;
}

template<class T>
Node<T> *LinkedList<T>::getLastNode() const {
    
    Node<T> * teze = head;
    for(int i = 1; i < size; i++)
    {
        teze = teze -> next ;
    }
    return teze ;
}

template<class T>
Node<T> *LinkedList<T>::getNode(const T &data) const {
    
    Node<T> * sungerbob = head ;
    for(int i = 0; i < size; i++)
    {
        if(sungerbob -> data == data) return sungerbob ;
        sungerbob = sungerbob -> next ;
    }
    
    return NULL ;
}

template<class T>
Node<T> *LinkedList<T>::getNodeAtIndex(int index) const {
    
    Node <T> * fitzgerald = head ;
    for(int i = 0; i < size; i++)
    {
        if(i == index) return fitzgerald ;
        fitzgerald = fitzgerald -> next ;
    }
    
    return NULL ;
}

template<class T>
void LinkedList<T>::insertAtTheFront(const T &data) {
    
    Node <T> * bitch = new Node<T>(data) ;
    if(head == NULL)
    {
        bitch -> prev = bitch ;
        bitch -> next = bitch ;
    }
    
    else 
    {
        bitch -> prev = head -> prev ;
        bitch -> next = head ;
        head -> prev = bitch ; 
        bitch -> prev -> next = bitch ;
    }

    head = bitch ;
    size++;
    

}

template<class T>
void LinkedList<T>::insertAtTheEnd(const T &data) {
    
    Node <T> * nice = new Node<T>(data) ;
    if(head == NULL)
    {
        nice -> prev = nice ;
        nice -> next = nice ;
        head = nice ;
    }  
    else
    {
        head -> prev -> next = nice ;
        nice -> prev = head -> prev ;
        head -> prev = nice ;
        nice -> next = head ;
    }  
    size++;
    
}

template<class T>
void LinkedList<T>::insertAfterNode(const T &data, Node<T> *node) {
    
    Node<T> * dog = head ;
    int does_the_node_exist = 0 ;
    for(int i = 0; i < size; i++)
    {
        if((dog -> data == node -> data) && (dog -> prev == node -> prev) && (dog -> next == node -> next)) { does_the_node_exist = 1; break ;}
        dog = dog -> next ;
    }
    
    if(does_the_node_exist == 1)
    {
        Node <T> * new_guy = new Node <T>(data) ;
        new_guy -> next = dog -> next ;
        new_guy -> prev = dog ;
        dog -> next = new_guy ;
        new_guy -> next -> prev = new_guy ;
        size++ ;
    }
    
}

template<class T>
void LinkedList<T>::insertAsEveryKthNode(const T &data, int k) {
    
    if (k < 2) ;
    else if ( head == NULL) ;
    else 
    {
        Node <T> * x = head ;
        int counter = 0 ;
        for(int i = 0, j = k -1; i < size; i++)
        {
            j--;
            if(j == 0)
            {
                Node <T> * new_guy = new Node <T>(data) ;
                new_guy -> next = x -> next ;
                new_guy -> prev = x ;
                x -> next = new_guy ;
                new_guy -> next -> prev = new_guy ;
                x = x -> next ;
                counter++ ; 
                j = k-1 ;           
            }
            x = x -> next ;
        }
        size += counter ;
    }
}

template<class T>
void LinkedList<T>::removeNode(Node<T> *node) {
    
    Node<T> * dog = head ;
    int does_the_node_exist = 0 ;
    for(int i = 0; i < size; i++)
    {
        if((dog -> data == node -> data) && (dog -> prev == node -> prev) && (dog -> next == node -> next)) { does_the_node_exist = 1; break ;}
        dog = dog -> next ;
    }
    if (does_the_node_exist == 1)
    {
        
        if(size == 1)
        {
            delete head ;
            head = NULL ;
            size-- ;
        }
        else
        {
            if(dog == head) head = head -> next ;
            dog -> prev -> next = dog -> next ;
            dog -> next -> prev = dog -> prev ;
            delete dog ;
            size-- ;
        }
    }
}

template<class T>
void LinkedList<T>::removeNode(const T &data) {
    
    Node<T> * dog = head, *tmp = NULL ;
    int counter = 0;
    for(int i = 0; i < size; i++)
    {
        if(dog -> data == data)
        { 
            tmp = dog -> next ;
            if(dog == head) head = tmp ;
            dog -> prev -> next = tmp ;
            tmp -> prev = dog -> prev ;
            delete dog ;
            dog = tmp ;
            counter++ ;   
            continue ;             
        }
        dog = dog -> next ;
    }
    size -= counter ;
    if(size == 0) head = NULL ;
}

template<class T>
void LinkedList<T>::removeAllNodes() {
    
    if (head == NULL) ;
    else 
    {
        Node <T> * destroyer = head, *tmp = NULL ;
        for(int i = 0; i < size; i++)
        {
            if (i != size - 1) tmp = destroyer -> next ;
            delete destroyer ;
            destroyer = tmp ;
        }
        head = NULL ;
        size = 0 ;
    }
}

template<class T>
void LinkedList<T>::removeEveryKthNode(int k) {
    
    if (k < 2 || head == NULL || size == 1) ;
    else 
    {
        Node <T> * x = head, *tmp = NULL ;
        int counter = 0 ;
        for(int i = 0, j = k ; i < size; i++)
        {
            j--;
            if(j == 0)
            {
                tmp = x -> next ;
                x -> prev -> next = tmp ;
                tmp -> prev = x -> prev ;
                delete x ;
                x = tmp ;
                counter++ ; 
                j = k ; 
                continue ;          
            }
            x = x -> next ;
        }
        size -= counter ;
    }
}

template<class T>
void LinkedList<T>::swap(Node<T> *node1, Node<T> *node2) {
    
    if(size < 2) ;
    else
    {
        Node<T> * dog = head, *n[2], *tmp[2] , *tmp_2[2]  ;
        int count = 0 ;
        for(int i = 0; i < size; i++)
        {
            if(((dog -> data == node1 -> data) && (dog -> prev == node1 -> prev) && (dog -> next == node1 -> next)) || ((dog -> data == node2 -> data) && (dog -> prev == node2 -> prev) && (dog -> next == node2 -> next))) 
            {
                n[count] = dog ;
                count++ ;
            }
            dog = dog -> next ;
        }
        if ( count == 2 )
        {
            if (size == 2) { head = n[2] ; return ; }
            tmp[0] = n[0] -> next ;
            tmp_2[0] = n[1] -> next ;
            tmp[1] = n[0] -> prev ;
            tmp_2[1] = n[1] -> prev ;
            if(tmp[0] == n[1]) 
            {
                n[1] -> next = n[1] ;
                n[0] -> prev = n[2] ;
            }
            else 
            {
                n[1] -> next = tmp[0] ;
                n[0] -> prev = tmp_2[1] ;
            }
            n[0] -> next = tmp_2[0] ;
            n[1] -> prev = tmp[1] ;
            n[0] -> next -> prev = n[0] ;
            n[0] -> prev -> next = n[0] ;
            n[1] -> next -> prev = n[1] ;
            n[1] -> prev -> next = n[1] ;
        }
        if(head == n[0]) head = n[1] ;
    }
}

template<class T>
void LinkedList<T>::shuffle(int seed) {
    
    if(size <= 1) return ;
    Node <T> * a = head , *b = head;
    int j, i ; 
    for(i = 0; i <= size - 1; i++)
    {
        b = head ;
        j = (i*i+seed) % size ;
        for(int k = 0; k < j ; k++)
        {
            b = b-> next ;
        }
        this -> swap(a,b) ;
        a = a -> next ;
    }
}

template<class T>
void LinkedList<T>::print(bool reverse) const {
    if (this->isEmpty()) {
        std::cout << "The list is empty." << std::endl;
        return;
    }

    if (reverse) {
        // traverse in reverse order (last node to first node).

        Node<T> *node = this->getLastNode();

        do {
            std::cout << *node << std::endl;
            node = node->prev;
        }
        while (node != this->getLastNode());
    } else {
        // traverse in normal order (first node to last node).

        Node<T> *node = this->getFirstNode();

        do {
            std::cout << *node << std::endl;
            node = node->next;
        }
        while (node != this->getFirstNode());
    }
}

template<class T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T> &rhs) {
    
    if (head == NULL) ;
    else 
    {
        Node <T> * destroyer = head, *tmp = NULL ;
        for(int i = 0; i < size; i++)
        {
            if (i != size - 1) tmp = destroyer -> next ;
            delete destroyer ;
            if (i != size - 1) destroyer = tmp ;
        }
        head = NULL ;
        size = 0 ;
    }

    if (rhs.head == NULL) return *this ;
    else
    {
        int s = rhs.size ;
        Node <T> * zidane = rhs.head, *tmp, *hey = NULL ;
        head = new Node <T>(zidane -> data) ;
        tmp = head ;
        head -> next = head ;
        zidane = zidane -> next ;
        size++;
        for(int i = 1; i < s; i++)
        {
            hey = new Node <T>(zidane -> data) ;
            hey -> prev = tmp ;
            tmp -> next = hey ;
            tmp = hey ; 
            zidane = zidane -> next ;
            size++;
        }
        if (s != 1) hey -> next = head ;
        head -> prev = tmp ;
    }
    return *this ;
    
}

#endif //LINKEDLIST_H
