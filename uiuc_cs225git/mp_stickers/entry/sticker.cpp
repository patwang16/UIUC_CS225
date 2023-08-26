#include "Image.h"
#include "StickerSheet.h"

int main() {
  //
  // Reminder:
  //   Before exiting main, save your creation to disk as myImage.png
  //
  Image image, penguin, bigI;
  image.readFromFile("../data/alma.png");
  penguin.readFromFile("../data/penguin.png");
  bigI.readFromFile("../data/i.png");
  StickerSheet myImage(image, 3);
  myImage.addSticker(penguin, 600, 100);
  myImage.addSticker(bigI, 200, 200);
  myImage.addSticker(penguin, 100, 100);
  myImage.render().writeToFile("../myImage.png");
  return 0;
}
