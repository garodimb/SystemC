
#ifndef COMPLEX_H_
#define COMPLEX_H_

#include <ostream>

class Complex {
    public:
        Complex() :
        rel_(0),
        img_(0) {}
        Complex(float rel, float img) :
        rel_(rel),
        img_(img) { }

        float rel() const { return rel_; }
        void set_rel(float rel) { rel_ = rel; }

        float img() const { return img_; }
        void set_img(float img) { img_ = img; }

    private:
        friend std::ostream &operator<<( std::ostream &output, 
         const Complex &comp) { 
         output << comp.rel_ << " + " << comp.img_ << "i";
         return output;            
        }

        float img_;
        float rel_;
};

#endif  // COMPLEX_H_