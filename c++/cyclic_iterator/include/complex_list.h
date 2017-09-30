/**
 * Provides cyclic iterator on ComplexList
 * see cyclic_iterator.h and complex.h for more info
 */

#ifndef COMPLEX_LIST_H_
#define COMPLEX_LIST_H_

#include <vector>
#include "cyclic_iterator.h"

class ComplexList {
    public:

        ComplexList(size_t count) :
        count_(count),
        cmplx_list_(0) { }
        
        void push_back(const Complex& complex) {
            cmplx_list_.push_back(complex);
        }
    
    typedef cyclic_iterator<std::vector<Complex>::iterator> iterator;
    typedef cyclic_iterator<std::vector<Complex>::const_iterator> const_iterator;

    iterator begin() {
        return iterator(cmplx_list_.begin(),cmplx_list_.end(),count_);
    }

    iterator end() {
        iterator iter(cmplx_list_.end());
        return iter;
    }
    
    private:
        std::vector<Complex> cmplx_list_;
        size_t count_;
};
#endif // COMPLEX_LIST_H_