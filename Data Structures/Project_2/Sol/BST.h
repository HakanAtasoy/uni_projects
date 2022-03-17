#ifndef BST_H
#define BST_H

#include <iostream>

#include "BSTNode.h"

enum TraversalPlan {preorder, inorder, postorder};

template<class T>
class BST {
public: // DO NOT CHANGE THIS PART.
    BST();
    BST(const BST<T> &obj);

    ~BST();

    BSTNode<T> *getRoot() const;
    bool isEmpty() const;
    bool contains(BSTNode<T> *node) const;

    void insert(const T &data);

    void remove(const T &data);
    void removeAllNodes();

    BSTNode<T> *search(const T &data) const;
    BSTNode<T> *getSuccessor(BSTNode<T> *node, TraversalPlan tp) const;

    void print(TraversalPlan tp=inorder) const;

    BST<T> &operator=(const BST<T> &rhs);

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.
    void print(BSTNode<T> *node, TraversalPlan tp) const;

    void constructor_helper(BSTNode<T>*& x, BSTNode<T>* y) ;

    void destructor_helper(BSTNode<T> * kekw) ;

    bool contains_helper(BSTNode<T> * lol, BSTNode<T> * lul) const ; 

    void insert_helper(BSTNode<T> *& rose ,const T & data) ;

    void remove_helper(BSTNode<T>*& jennie, const T & data) ;

    T remove_helper_helper(BSTNode<T>*& jisoo) ;

    BSTNode<T>* search_helper(BSTNode<T>* lisa, const T & data) const ;
    
    void getSuccessor_helper(BSTNode<T>* twice, BSTNode<T>*& thrice, BSTNode<T>* fourth, TraversalPlan tp) const ;

    void getSuccessor_helper(BSTNode<T>* joy, BSTNode<T>* irene, TraversalPlan wendy, bool & succeed, BSTNode<T> *& seulgi) const ;

    void removeAllNodes_helper(BSTNode<T>* so) ;

private: // DO NOT CHANGE THIS PART.
    BSTNode<T> *root;
};

#endif //BST_H

template<class T>
BST<T>::BST() {
    
    root = NULL ;
}

template<class T>
BST<T>::BST(const BST<T> &obj) {
    
    this -> root = NULL ;
    constructor_helper(this -> root, obj.getRoot()) ;
}

template<class T>
BST<T>::~BST() {
    
    destructor_helper(root) ;
    root = NULL ;
}

template<class T>
BSTNode<T> *BST<T>::getRoot() const {
    
    return root ;
}

template<class T>
bool BST<T>::isEmpty() const {
    
    if ( root ) return false ;
    else return true ;
}

template<class T>
bool BST<T>::contains(BSTNode<T> *node) const {
    
    if (!root) return false ;
    else return contains_helper(root, node) ;
}

template<class T>
void BST<T>::insert(const T &data) {
    
    insert_helper(root, data) ;
}

template<class T>
void BST<T>::remove(const T &data) {
    
    remove_helper(root, data) ;
}

template<class T>
void BST<T>::removeAllNodes() {
    
    destructor_helper(root) ;
    root == NULL ;
}

template<class T>
BSTNode<T> *BST<T>::search(const T &data) const {
    
    return search_helper(root, data) ;
}

template<class T>
BSTNode<T> *BST<T>::getSuccessor(BSTNode<T> *node, TraversalPlan tp) const {

    bool success = false ;
    BSTNode<T> * seulgi = NULL ;
    getSuccessor_helper(this -> root, tp, success, seulgi) ;
    return seulgi ;
}

template<class T>
void BST<T>::print(TraversalPlan tp) const {

    if (tp == inorder) {
        // check if the tree is empty?
        if (isEmpty()) {
            // the tree is empty.
            std::cout << "BST_inorder{}" << std::endl;
            return;
        }

        // the tree is not empty.

        // recursively output the tree.
        std::cout << "BST_inorder{" << std::endl;
        print(root, inorder);
        std::cout << std::endl << "}" << std::endl;
    } else if (tp == preorder) {

        if (isEmpty()) {
            
            std::cout << "BST_preorder{}" << std::endl;
            return;
        }

        std::cout << "BST_preorder{" << std::endl;
        print(root, preorder);
        std::cout << std::endl << "}" << std::endl;
    } else if (tp == postorder) {
        
        if (isEmpty()) {
            
            std::cout << "BST_postorder{}" << std::endl;
            return;
        }

        std::cout << "BST_postorder{" << std::endl;
        print(root, postorder);
        std::cout << std::endl << "}" << std::endl;
    }
}

template<class T>
BST<T> &BST<T>::operator=(const BST<T> &rhs) {
    if(this -> root) this -> removeAllNodes() ;
    constructor_helper(this, rhs) ;
}

template<class T>
void BST<T>::print(BSTNode<T> *node, TraversalPlan tp) const {

    // check if the node is NULL?
    if (node == NULL)
        return;

    if (tp == inorder) {
        // first, output left subtree and comma (if needed).
        print(node->left, inorder);
        if (node->left) {
            std::cout << "," << std::endl;
        }

        // then, output the node.
        std::cout << "\t" << node->data;

        // finally, output comma (if needed) and the right subtree.
        if (node->right) {
            std::cout << "," << std::endl;
        }
        print(node->right, inorder);
    } 
    else if (tp == preorder) {

        std::cout << "\t" << node->data ;
        
        if (node->left) {
            std::cout << "," << std::endl;
        }

        print(node->left, preorder);

        if (node->right) {
            std::cout << "," << std::endl;
        }
        print(node->right, preorder);    
    }      
    else if (tp == postorder) {  

        print(node->left, postorder);
        if (node->left) {
            std::cout << "," << std::endl;
        }

        print(node->right, postorder);
        if (node->right) {
            std::cout << "," << std::endl;
        }    

        std::cout << "\t" << node->data ;
          
    }
}



template<class T>
void BST<T>::constructor_helper(BSTNode<T>*& x, BSTNode<T>* y)
{
    if (y)
    {
        x = new BSTNode<T>();
        x -> data = y -> data ;
        constructor_helper(x -> left, y -> left) ;
        constructor_helper(x -> right, y -> right) ;
    }
    
    else ;
}

template<class T>
void BST<T>::destructor_helper(BSTNode<T> * kekw)
{
    if(kekw)
    {
        destructor_helper(kekw -> left) ;
        destructor_helper(kekw -> right) ;
        delete kekw ;
        kekw = NULL ;
    }
    else ;
}

template<class T>
bool BST<T>::contains_helper(BSTNode<T> * lol, BSTNode<T> * lul) const
{
    if(!lol) return false ;
    else
    {
        if ( lol -> data > lul -> data ) return contains_helper( lol -> left, lul) ;
        else if ( lol -> data < lul -> data ) return contains_helper( lol -> right, lul) ;
        else if ( lol -> data == lul -> data && lol -> left == lul -> left && lol -> right == lul -> right ) return true ;
    }
}

template<class T>
void BST<T>::insert_helper(BSTNode<T> *& rose ,const T & data)
{
    if(!rose)
    {
        rose = new BSTNode<T>() ;
        rose -> data = data ;
    }

    else if ( rose -> data > data ) insert_helper(rose -> left, data) ;
    else if ( rose -> data < data ) insert_helper(rose -> right, data) ;
}


template<class T>
void BST<T>::remove_helper(BSTNode<T> *& jennie ,const T & data)
{
    if(!jennie) ;
    else if(jennie -> data == data)
    {
        if(jennie -> right == NULL && jennie -> left == NULL)
        {
            delete jennie ;
            jennie = NULL ;
        }
        else if(jennie -> right == NULL && jennie -> left != NULL)
        {
            BSTNode<T> * tmp = jennie ;
            jennie = jennie -> left ;
            delete tmp ;
        } 
        else if(jennie -> left == NULL && jennie -> right != NULL)
        {
            BSTNode<T> * tmp = jennie ;
            jennie = jennie -> right ;
            delete tmp ;
        }
        else
        {
            jennie -> data = remove_helper_helper(jennie -> right) ;   
        }
        
    }
    else if(jennie -> data > data) remove_helper(jennie -> left, data) ;
    else if(jennie -> data < data) remove_helper(jennie -> right, data) ;
}


template<class T>
T BST<T>::remove_helper_helper(BSTNode<T> *& jisoo) 
{
    if(!jisoo -> left)
    {
        T tmp = jisoo -> data ;
        if(!jisoo -> right) { delete jisoo ; jisoo = NULL ;}
        else 
        {
            BSTNode<T> * tmpp = jisoo ;
            jisoo = jisoo -> right ;
            delete tmpp ;
        }
        return tmp ;
    }
    else remove_helper_helper(jisoo -> left) ;
}


template<class T>
BSTNode<T>* BST<T>::search_helper(BSTNode<T>* lisa, const T & data) const
{
    if(!lisa) return NULL ;
    else if (lisa -> data == data) return lisa ;
    else if (lisa -> data > data ) search_helper(lisa -> left, data) ;
    else if (lisa -> data < data ) search_helper(lisa -> right, data) ;
}


template<class T>
void BST<T>::getSuccessor_helper(BSTNode<T>* joy, BSTNode<T>* irene, TraversalPlan wendy, bool & succeed) const ;
{
    
    if (joy == NULL)
        return ;

    if (tp == inorder) {
        
        getSuccessor_helper(joy->left, inorder);
        
        if(succeed) seulgi = joy ;
        if((joy -> data == irene -> data) && (joy -> left == irene -> left) && (joy -> right == irene -> right)) succeed = true;

        getSuccessor_helper(joy->right, inorder);
    } 
    else if (tp == preorder) {

        if(succeed) seulgi = joy ;
        if((joy -> data == irene -> data) && (joy -> left == irene -> left) && (joy -> right == irene -> right)) succeed = true;

        getSuccessor_helper(twice->left, preorder) ;

        getSuccessor_helper(twice->right, preorder) ;   
    }      
    else if (tp == postorder) {  

        getSuccessor_helper(twice->left, postorder);

        getSuccessor_helper(twice->right, postorder);

        if(succeed) seulgi = joy ;
        if((joy -> data == irene -> data) && (joy -> left == irene -> left) && (joy -> right == irene -> right)) succeed = true;
          
    }
}


template<class T>
void BST<T>::removeAllNodes_helper(BSTNode<T>* so)
{
    if(!so) ;
    else 
    {
        removeAllNodes_helper(so -> left) ;
        removeAllNodes_helper(so -> right) ;
        this -> remove(so -> data) ;
    }
}











