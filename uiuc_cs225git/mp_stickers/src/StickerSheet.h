/**
 * @file StickerSheet.h
 * Contains your declaration of the interface for the StickerSheet class.
 */
#pragma once
#include "Image.h"
using namespace std;
#include <vector>

class Sticker{
    public:
        Image* image_;
        int x_pos;
        int y_pos;

        Sticker();
        Sticker(Image& image, int x, int y);
        Sticker(Sticker& other);
};


class StickerSheet{
    public:
        StickerSheet(const Image &picture, unsigned max);
        StickerSheet(const StickerSheet &other);
        const StickerSheet & operator=(const StickerSheet &other);
        ~StickerSheet(); //destructor for rule of three
        void changeMaxStickers(unsigned max);
        int addSticker(Image &sticker, int x, int y);
        int setStickerAtLayer(Image &sticker, unsigned layer, int x, int y);
        bool translate(unsigned index, int x, int y);
        void removeSticker(unsigned index);
        Image* getSticker(unsigned index);
        int layers() const;
        Image render() const;
        vector<Sticker*>* getStickers() const;
        Image* getBase() const;
    private:
        Image* base_;
        unsigned int max_layers;
        vector<Sticker*>* stickers;
};