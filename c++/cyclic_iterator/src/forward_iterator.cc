/**
 * Prerequisite: Boost Library
 * Build: g++ -std=c++11 forward_iterator.cc -o forward_iterator
 */
#include <iostream>
#include <vector>
#include <boost/foreach.hpp>

#include "complex.h"
#include "cyclic_iterator.h"
#include "complex_list.h"


int main(int argc, char const *argv[])
{
    ComplexList cmplx_list(7);
    for (int i = 0; i < 5; ++i)
    {
        cmplx_list.push_back(*new Complex(i,i));
    }

    int i = 1;
    std::cout << "*****************" << std::endl;
    std::cout << "STL Style" << std::endl;
    ComplexList::iterator iter = cmplx_list.begin();
    for( i = 1; iter != cmplx_list.end(); ++iter,i++){
        std::cout << "[ " << i << " ] " << (*iter) << std::endl;
    }

    
    // -std=c++11
    std::cout << "*****************" << std::endl;
    std::cout << "C++11 Style" << std::endl;
    i = 1;
    for(Complex comp : cmplx_list){
        std::cout << "[ " << i++ << " ] " << comp << std::endl;
    }

    // Boost
    std::cout << "*****************" << std::endl;
    std::cout << "BOOST Style" << std::endl;
    i = 1;
    BOOST_FOREACH( Complex comp, cmplx_list )
    {
        std::cout << "[ " << i++ << " ] " << comp << std::endl;
    }

    std::vector<Complex> vect_complex(cmplx_list.begin(),cmplx_list.end());
    std::cout << "*****************" << std::endl;
    std::cout << "vector Initialization" << std::endl;
    i = 1;
    for(Complex comp : vect_complex){
        std::cout << "[ " << i++ << " ] " << comp << std::endl;
    }
    return 0;
}