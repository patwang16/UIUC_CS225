#include "StickerSheet.h"
//Sticker Class
Sticker::Sticker(){
    image_ = NULL;
    x_pos = 0;
    y_pos = 0;
}

Sticker::Sticker(Image& image, int x, int y){
    x_pos = x;
    y_pos = y;
    image_ = &image;
}

Sticker::Sticker(Sticker& other){
    image_ = other.image_;
    x_pos = other.x_pos;
    y_pos = other.y_pos;
}

//StickerSheet

StickerSheet::StickerSheet(const Image &picture, unsigned max){
    base_ = new Image(picture);
    max_layers = max;
    stickers = new vector<Sticker*>;
}

StickerSheet::StickerSheet(const StickerSheet &other){
    base_ = new Image(*(other.getBase()));
    max_layers = other.layers();
    stickers = new vector<Sticker*>;
    for(unsigned int i = 0; i < other.getStickers()->size(); i++){
        (*stickers).push_back(new Sticker(*((*(other.getStickers()))[i])));
    }
}

const StickerSheet & StickerSheet::operator=(const StickerSheet &other){
    delete base_;
    for (unsigned int i = 0; i < stickers->size(); i++){
        delete (*stickers)[i];
    }
    delete stickers;
    base_ = new Image(*(other.getBase()));
    max_layers = other.layers();
    stickers = new vector<Sticker*>;
    for (unsigned int i = 0; i < other.getStickers()->size(); i++){
        (*stickers).push_back(new Sticker(*((*(other.getStickers()))[i])));
    }
    return (*this);
}

StickerSheet::~StickerSheet(){ //destructor for rule of three
    delete base_;
    for (unsigned int i = 0; i < stickers->size(); i++){
        delete (*stickers)[i];
    }
    delete stickers;
}
void StickerSheet::changeMaxStickers(unsigned max){
    if (max < stickers->size()){
        for (unsigned int i = max; i < stickers->size(); i++){
            delete (*stickers)[i];
        }
        stickers->resize(max);
    }
    max_layers = max;
}
int StickerSheet::addSticker(Image &sticker, int x, int y){
    if (stickers->size() >= max_layers){
        changeMaxStickers(max_layers + 1);
    }
    Sticker* temp = new Sticker(sticker, x, y);
    stickers->push_back(temp);
    return stickers->size()-1;
}
int StickerSheet::setStickerAtLayer(Image &sticker, unsigned layer, int x, int y){
    if (layer < stickers->size()){
        delete (*stickers)[layer];
        (*stickers)[layer] = new Sticker(sticker, x, y);
        return layer;
    }
    return -1;
    
}
bool StickerSheet::translate(unsigned index, int x, int y){
    if (index >= stickers->size() || (*stickers)[index]->image_ == NULL){
        return false;
    }
    (*stickers)[index]->x_pos = x;
    (*stickers)[index]->y_pos = y;
    return true;
}

void StickerSheet::removeSticker(unsigned index){
    delete (*stickers)[index];
    stickers->erase(stickers->begin() + index);
}

Image* StickerSheet::getSticker(unsigned index){
    if (index < stickers->size()){
        return (*stickers)[index]->image_;
    }
    return NULL;
}

Image StickerSheet::render() const{
    Image final = Image(*base_);
    int min_x = 0;
    int min_y = 0;
    int max_x = int(final.width());
    int max_y = int(final.height());
    for (unsigned int i = 0; i < stickers->size(); i++){
        Sticker* tempSticker = (*stickers)[i];
        //std::cout << tempSticker->x_pos << tempSticker->y_pos << std::endl;
        min_x = min(min_x, tempSticker->x_pos);
        min_y = min(min_y, tempSticker->y_pos);
        max_x = max(max_x, tempSticker->x_pos + int(tempSticker->image_->width()));
        max_y = max(max_y, tempSticker->y_pos + int(tempSticker->image_->height()));
    }
    int base_x = abs(min_x);
    int base_y = abs(min_y);
    Image temp_copy = Image(final);
    final.resize(max_x - min_x, max_y - min_y);
    
    
    for (int i = 0; i < int(final.width()); i++){
        for (int j = 0; j < int(final.height()); j++){
            if (i >= base_x && i < base_x + int(temp_copy.width()) && j >= base_y && j < base_y + int(temp_copy.height())){
                final.getPixel(i,j) = temp_copy.getPixel(i - base_x, j - base_y);
            }
            else{
                final.getPixel(i,j).s = 0.0;
                final.getPixel(i,j).h = 0.0;
                final.getPixel(i,j).l = 1;
                final.getPixel(i,j).a = 1;
            }
        }
    }
    for (unsigned int t = 0; t < stickers->size(); t++){
        Sticker* tempSticker = (*stickers)[t];
        for (int i = tempSticker->x_pos + base_x; i < tempSticker->x_pos+base_x+int(tempSticker->image_->width()); i++){
            for (int j = tempSticker->y_pos + base_y; j < tempSticker->y_pos+base_y+int(tempSticker->image_->height()); j++){
                cs225::HSLAPixel & topPixel = tempSticker->image_->getPixel(i - (tempSticker->x_pos + base_x), j - (tempSticker->y_pos + base_y));
                if (topPixel.a == 0){
                    continue;
                }
                cs225::HSLAPixel & bottomPixel = final.getPixel(i, j);
                bottomPixel = topPixel;
            }   
        }
    }
    return final;
}

int StickerSheet::layers() const{
    return max_layers;
}
vector<Sticker*>* StickerSheet::getStickers() const{
    return stickers;
}
Image* StickerSheet::getBase() const{
    return base_;
}