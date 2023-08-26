#include "Image.h"

void Image::lighten(){
    for (unsigned int i = 0; i < width(); i++){
        for (unsigned int j = 0; j < height(); j++){
            cs225::HSLAPixel & temp = this->getPixel(i, j);
            double newLum = temp.l + 0.1;
            if (newLum > 1){
                temp.l = 1;
            }
            else{
                temp.l = newLum;
            }
        }
    }
}

void Image::lighten(double amount){
    for (unsigned int i = 0; i < width(); i++){
        for (unsigned int j = 0; j < height(); j++){
            cs225::HSLAPixel & temp = this->getPixel(i, j);
            double newLum = temp.l + amount;
            if (newLum > 1){
                temp.l = 1;
            }
            else{
                temp.l = newLum;
            }
        }
    }
}

void Image::darken(){
    for (unsigned int i = 0; i < width(); i++){
        for (unsigned int j = 0; j < height(); j++){
            cs225::HSLAPixel & temp = this->getPixel(i, j);
            double newLum = temp.l - 0.1;
            if (newLum < 0){
                temp.l = 0;
            }
            else{
                temp.l = newLum;
            }
        }
    }
}

void Image::darken(double amount){
    for (unsigned int i = 0; i < width(); i++){
        for (unsigned int j = 0; j < height(); j++){
            cs225::HSLAPixel & temp = this->getPixel(i, j);
            double newLum = temp.l - amount;
            if (newLum < 0){
                temp.l = 0;
            }
            else{
                temp.l = newLum;
            }
        }
    }
}

void Image::saturate(){
    for (unsigned int i = 0; i < width(); i++){
        for (unsigned int j = 0; j < height(); j++){
            cs225::HSLAPixel & temp = this->getPixel(i, j);
            double newSat = temp.s + 0.1;
            if (newSat > 1){
                temp.s = 1;
            }
            else{
                temp.s = newSat;
            }
        }
    }
}

void Image::saturate(double amount){
    for (unsigned int i = 0; i < width(); i++){
        for (unsigned int j = 0; j < height(); j++){
            cs225::HSLAPixel & temp = this->getPixel(i, j);
            double newSat = temp.s + amount;
            if (newSat > 1){
                temp.s = 1;
            }
            else{
                temp.s = newSat;
            }
        }
    }
}

void Image::desaturate(){
    for (unsigned int i = 0; i < width(); i++){
        for (unsigned int j = 0; j < height(); j++){
            cs225::HSLAPixel & temp = this->getPixel(i, j);
            double newSat = temp.s - 0.1;
            if (newSat < 0){
                temp.s = 0;
            }
            else{
                temp.s = newSat;
            }
        }
    }
}

void Image::desaturate(double amount){
    for (unsigned int i = 0; i < width(); i++){
        for (unsigned int j = 0; j < height(); j++){
            cs225::HSLAPixel & temp = this->getPixel(i, j);
            double newSat = temp.s - amount;
            if (newSat < 0){
                temp.s = 0;
            }
            else{
                temp.s = newSat;
            }
        }
    }
}

void Image::grayscale(){
    for (unsigned int i = 0; i < width(); i++){
        for (unsigned int j = 0; j < height(); j++){
            cs225::HSLAPixel & temp = this->getPixel(i, j);
            temp.s = 0;
        }
    }
}

void Image::rotateColor(double degrees){
    for (unsigned int i = 0; i < width(); i++){
        for (unsigned int j = 0; j < height(); j++){
            cs225::HSLAPixel & temp = this->getPixel(i, j);
            temp.h = temp.h + degrees;
            while (temp.h < 0){
                temp.h += 360;
            }
            while (temp.h > 360){
                temp.h -= 360;
            }
        }
    }
}

void Image::illinify(){
    for (unsigned int i = 0; i < width(); i++){
        for (unsigned int j = 0; j < height(); j++){
            cs225::HSLAPixel & temp = this->getPixel(i, j);
            int distBlue = min(216 - temp.h, temp.h-216);
            int distOrange = min(11 - temp.h, temp.h - 11);
            if (distBlue < distOrange){
                temp.h = 216;
            }
            else {
                temp.h = 11;
            }
        }
    }
}

void Image::scale(double factor){
    cs225::PNG temp_copy = PNG(*this);
    resize(unsigned(width()*factor), unsigned(height()*factor));
    for (unsigned int i = 0; i < width(); i++){
        for (unsigned int j = 0; j < height(); j++){
            this->getPixel(i, j) = temp_copy.getPixel(unsigned(i / factor), unsigned(j / factor));
        }
    }
}

void Image::scale(unsigned w, unsigned h){
    double minFactor = min(double(w)/double(width()), double(h)/double(height()));
    scale(minFactor);
}