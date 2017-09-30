/**
 * Cyclic iterator implementation. Iterate on count_ number of elements.
 * If count_ is more than number of elements in container(IteratorBase is iterator over
 * original container) then it starts from begining.
 * Usage: e.g. 
 * typedef cyclic_iterator<std::vector<T>::iterator> iterator;
 * typedef cyclic_iterator<std::vector<T>::const_iterator> const_iterator;
 * For example see: complex_list.h and complex.h
 */

#ifndef CYCLIC_ITERATOR_H_
#define CYCLIC_ITERATOR_H_

#include <boost/iterator/iterator_adaptor.hpp>

template <typename IteratorBase>
class cyclic_iterator :
        public boost::iterator_adaptor<
                cyclic_iterator<IteratorBase>,
                IteratorBase,
                boost::use_default,
                std::forward_iterator_tag,
                boost::use_default
            >
{

 private:
    friend class boost::iterator_core_access;
    IteratorBase it_begin_, it_end_;
    size_t count_;

 public:
    explicit cyclic_iterator(IteratorBase iter) :
        it_begin_(iter),
        it_end_(iter),
        count_(0),
        cyclic_iterator::iterator_adaptor_(iter) {}

    cyclic_iterator(IteratorBase it_begin, IteratorBase it_end, size_t count) :
        it_begin_(it_begin),
        it_end_(it_end),
        count_(count),
        cyclic_iterator::iterator_adaptor_(it_begin) {
            if(count_ == 0) {
                it_begin_ = it_end_;
                this->base_reference() = it_end_;
            }
            else{
                count_--;
            }
        }

    void increment() {

        // End of cyclic_iterator
        if(count_ == 0) {
            this->base_reference() = it_end_;
            return ;
        }

        // Extra precauition, no increment after end
        if(this->base_reference() == it_end_) {
             return;
        }

        --count_;
        ++(this->base_reference());

        // Start from begin
        if( this->base_reference() == it_end_) {
            this->base_reference() = it_begin_;
        }
    }

};

#endif // CYCLIC_ITERATOR_H_