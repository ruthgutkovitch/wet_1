#ifndef FIRST_ASSIGNMENT_AVLTREEIMP_H
#define FIRST_ASSIGNMENT_AVLTREEIMP_H

#include <iostream>
#include "AVLtree.h"

//compare 2 vertex and return the max height
template <class T>
int max(const Vertex<T>* v1, const Vertex<T>* v2){
    int heightV1;
    int heightV2;
    if(v1 == nullptr){
        heightV1 = -1;
    }else{
        heightV1 = v1->GetVertexHeight();
    }
    if(v2 == nullptr){
        heightV2 = -1;
    }else{
        heightV2 = v2->GetVertexHeight();
    }
    if(heightV1 > heightV2){
        return heightV1;
    }
    return heightV2;
}

//delete all the tree vertex with recursion
template <class T>
void AVLtree<T>::DeleteAVLtree(Vertex<T>* vertex){
    if(vertex == nullptr){
        return;
    }

    DeleteAVLtree(vertex->leftSon);
    DeleteAVLtree(vertex->rightSon);

    delete vertex->data;
    delete vertex;
}

//destructor
template <class T>
AVLtree<T>::~AVLtree(){
    DeleteAVLtree(treeRoot);
}

template <class T>
void AVLtree<T>::AddVertex(Vertex<T>* v, const T& data){
    treeRoot = AddVertexAux(v, data);
    treeRoot->father = nullptr;
}

///adds new vertex in the correct place in the tree and then check balance and rotate
///log(numOfVertexInTree)
template <class T>
Vertex<T>* AVLtree<T>::AddVertexAux(Vertex<T>* v, const T& data){
    if(v == nullptr){
        Vertex<T>* newVertex = new Vertex<T>();
        newVertex->data = new T(data);
        return newVertex;
    }

    if(data < *v->data){
        v->leftSon = AddVertexAux(v->leftSon, data);
        if(v->leftSon != nullptr){
            v->leftSon->father = v;
        }
    }
    if(data > *(v->data)){
        v->rightSon = AddVertexAux(v->rightSon, data);
        if(v->rightSon != nullptr){
            v->rightSon->father = v;
        }
    }
    if(data == *(v->data)){
        throw VertexAlreadyExist();
    }

    v->height = max(v->leftSon, v->rightSon) + 1;

    int balance = CalculateBalance(v);

    if(balance == 2 && CalculateBalance(v->leftSon) >= 0){
        v = RotationLL(v);
    }else if(balance == 2 && CalculateBalance(v->leftSon) == -1){
        v = RotationLR(v);
    }else if(balance == -2 && CalculateBalance(v->rightSon) <= 0){
        v = RotationRR(v);
    }else if(balance == -2 && CalculateBalance(v->rightSon) == 1){
        v = RotationRL(v);
    }

    return v;
}

template <class T>
void AVLtree<T>::RemoveVertex(Vertex<T>* v, const T& data){
    treeRoot = RemoveVertexAux(v, data);
    if(treeRoot != nullptr){
        treeRoot->father = nullptr;
    }
}

//removes specific vertex in the the tree and then check balance and rotate
///log(numOfVertexInTree)
template <class T>
Vertex<T>* AVLtree<T>::RemoveVertexAux(Vertex<T>* v, const T& data){
    if(v == nullptr){
        return nullptr;
    }
    if(data < *v->data){
        v->leftSon = RemoveVertexAux(v->leftSon, data);
        if(v->leftSon != nullptr){
            v->leftSon->father = v;
        }
    }
    else if(data > *v->data){
        v->rightSon = RemoveVertexAux(v->rightSon, data);
        if(v->rightSon != nullptr){
            v->rightSon->father = v;
        }
    }
    else if(data == *v->data){
        if(v->leftSon == nullptr && v->rightSon == nullptr){///if v has no sons
            delete v->data;
            delete v;
            return nullptr;
        }
        else if(v->leftSon == nullptr || v->rightSon == nullptr){///if v has one son
            if(v->leftSon == nullptr) {
                T* temp = v->data;
                v->data = v->rightSon->data;
                v->rightSon->data = temp;
                delete v->rightSon->data;
                delete v->rightSon;
                v->rightSon = nullptr;
            }else{
                T* temp = v->data;
                v->data = v->leftSon->data;
                v->leftSon->data = temp;
                delete v->leftSon->data;
                delete v->leftSon;
                v->leftSon = nullptr;
            }
        }else{///if v has 2 sons
            Vertex<T>* toDelete = GetMin(v->rightSon);
            T* temp = v->data;
            v->data = toDelete->data;
            toDelete->data = temp;
            v->rightSon = RemoveVertexAux(v->rightSon, *toDelete->data);
            if(v->rightSon != nullptr){
                v->rightSon->father = v;
            }
        }
    }

    v->height = max(v->leftSon, v->rightSon) + 1;

    int balance = CalculateBalance(v);

    if(balance == 2 && CalculateBalance(v->leftSon) >= 0){
        return RotationLL(v);
    }

    if(balance == 2 && CalculateBalance(v->leftSon) == -1){
        return RotationLR(v);
    }

    if(balance == -2 && CalculateBalance(v->rightSon) <= 0){
        return RotationRR(v);
    }

    if(balance == -2 && CalculateBalance(v->rightSon) == 1){
        return RotationRL(v);
    }

    return v;

};

///O(1)
template <class T>
Vertex<T>* AVLtree<T>::RotationLL(Vertex<T>* v){
    Vertex<T>* tempLeftSon = v->leftSon;
    v->leftSon = tempLeftSon->rightSon;
    if(tempLeftSon->rightSon != nullptr){
        v->leftSon->father = v;
    }
    tempLeftSon->rightSon = v;
    v->father = tempLeftSon;

    v->height = max(v->leftSon, v->rightSon) + 1;
    tempLeftSon->height = max(tempLeftSon->leftSon, tempLeftSon->rightSon) + 1;

    return tempLeftSon;
}

///O(1)
template <class T>
Vertex<T>* AVLtree<T>::RotationLR(Vertex<T>* v){
    v->leftSon = RotationRR(v->leftSon);
    v->leftSon->father = v;

    return RotationLL(v);
}

///O(1)
template <class T>
Vertex<T>* AVLtree<T>::RotationRR(Vertex<T>* v){
    Vertex<T>* tempRightSon = v->rightSon;
    v->rightSon = tempRightSon->leftSon;
    if(tempRightSon->leftSon != nullptr){
        v->rightSon->father = v;
    }
    tempRightSon->leftSon = v;
    v->father = tempRightSon;

    v->height = max(v->leftSon, v->rightSon) + 1;
    tempRightSon->height = max(tempRightSon->leftSon, tempRightSon->rightSon) + 1;
    return tempRightSon;
}

///O(1)
template <class T>
Vertex<T>* AVLtree<T>::RotationRL(Vertex<T>* v){
    v->rightSon = RotationLL(v->rightSon);
    v->rightSon->father = v;

    return RotationRR(v);
}

///search for data. return pointer if exist else return nullptr
///log(numOfVertexInTree)
template <class T>
Vertex<T>* AVLtree<T>::BinarySearch(Vertex<T>* v, const T& data) const{ ///data = lecture/course
    if(v == nullptr){
        return nullptr;
    }
    if(data == *v->data){
        return v;
    }
    if(data < *v->data){
        return BinarySearch(v->leftSon, data);
    }
    else{
        return BinarySearch(v->rightSon, data);
    }
}

///log(numOfVertexInTree)
//return pointer to vertex with max value in the tree
template<class T>
Vertex<T>* AVLtree<T>::GetMax(Vertex<T>* vertex) const {
    if(vertex != nullptr){
        while(vertex->rightSon != nullptr){
            vertex = vertex->rightSon;
        }
    }

    return vertex;
}

///log(numOfVertexInTree)
//return pointer to vertex with min value in the tree
template<class T>
Vertex<T>* AVLtree<T>::GetMin(Vertex<T>* vertex) const{
    if(vertex != nullptr){
        while(vertex->leftSon != nullptr){
            vertex = vertex->leftSon;
        }
    }

    return vertex;
}

///O(1)
//return pointer to root in the tree
template<class T>
Vertex<T>* AVLtree<T>::GetRoot() const{
    return this->treeRoot;
}

//print tree- used for our self-checking and for future uses
template<class T>
void AVLtree<T>::PrintTree(Vertex<T>* vertex) const{
    if(vertex == nullptr){
        return;
    }

    PrintTree(vertex->rightSon);
    std::cout << *(vertex->data) << " height: " << vertex->GetVertexHeight() << std::endl;
    PrintTree(vertex->leftSon);

}

//LEFT - going to left, RIGHT - going to right
///used only for courses
template<class T>
void AVLtree<T>::InorderAux(Vertex<T>* vertex, int num, SIDE indicator, int* count, int *courses, int *classes) const{
    if(vertex == nullptr || *count == num){
        return;
    }

    if(indicator == LEFT){///i came from my parent
        InorderAux(vertex->leftSon, num, LEFT,count, courses, classes);
        if(*count == num){
            return;
        }
        int arraySize = vertex->GetData()->GetNumOfClasses();
        for(int i = 0; i < arraySize; i++){
            if(vertex->GetData()->GetPtrLecture(i) == nullptr){
                *(courses+(*count)) = vertex->GetData()->GetCourseID();
                *(classes+(*count)) = i;
                *count += 1;
                if(*count == num){
                    return;
                }
            }
        }
        InorderAux(vertex->rightSon, num, LEFT,count, courses, classes);
        return;
    }

    if(indicator == RIGHT){///i came from my leftSon
        int arraySize = vertex->GetData()->GetNumOfClasses();
        for(int i = 0; i < arraySize; i++){
            if(vertex->GetData()->GetPtrLecture(i) == nullptr){
                *(courses+(*count)) = vertex->GetData()->GetCourseID();
                *(classes+(*count)) = i;
                *count += 1;
                if(*count == num){
                    return;
                }
            }
        }
        InorderAux(vertex->rightSon, num, LEFT,count, courses, classes);
        InorderAux(vertex->father, num, RIGHT, count, courses, classes);
        return;
    }

}

///used only for courses
//if there are not enough Lecture in lecturesTree tree for GetMostViewedClasses
template<class T>
void AVLtree<T>::Inorder(Vertex<T>* vertexStart, int num, int *courses, int *classes) const{
    if(vertexStart == nullptr){
        return;
    }
    int count = 0;
    InorderAux(vertexStart, num, RIGHT, &count, courses, classes);
}

///LEFT - going to left, RIGHT - going to right
///used only for lectures
template<class T>
void AVLtree<T>::ReverseInorderAux(Vertex<T> *vertex, int num, SIDE indicator, int* count, int *courses, int *classes) const{
    if(vertex == nullptr){
        return;
    }

    if((*count) == num){
        return;
    }

    if(indicator == RIGHT){///i came from my parent
        ReverseInorderAux(vertex->rightSon, num, RIGHT,count, courses, classes);
        if(*count == num){
            return;
        }
        *(courses+(*count)) = vertex->data->GetCourseID();
        *(classes+(*count)) = vertex->data->GetClassID();
        (*count)++;
        ReverseInorderAux(vertex->leftSon, num, RIGHT,count, courses, classes);
        return;
    }

    if(indicator == LEFT){///i came from my rightSon
        *(courses+(*count)) = vertex->data->GetCourseID();
        *(classes+(*count)) = vertex->data->GetClassID();
        (*count)++;
        ReverseInorderAux(vertex->leftSon, num, RIGHT,count, courses, classes);
        ReverseInorderAux(vertex->father, num, LEFT, count, courses, classes);
        return;
    }

}

///used only for lectures
template <class T>
void AVLtree<T>::ReverseInorder(Vertex<T>* vertexStart, int num, int *courses, int *classes) const{
    if(vertexStart == nullptr){
        return;
    }
    int count = 0;
    ReverseInorderAux(vertexStart, num, LEFT, &count, courses, classes);
}

// calculate balance for rotations after remove or add vertex
template <class T>
int AVLtree<T>::CalculateBalance(Vertex<T>* vertex) const{
    int balance = 0;
    int heightLeft = -1;
    int heightRight = -1;

    if(vertex != nullptr) {
        if (vertex->leftSon != nullptr){
            heightLeft = vertex->leftSon->height;
        }
        if(vertex->rightSon != nullptr){
            heightRight = vertex->rightSon->height;
        }
    }
    balance = heightLeft - heightRight;
    return balance;
}


#endif //FIRST_ASSIGNMENT_AVLTREEIMP_H