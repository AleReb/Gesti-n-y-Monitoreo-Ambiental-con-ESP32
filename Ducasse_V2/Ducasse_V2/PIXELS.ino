void changePixel(int r, int g, int b){
    pixels.clear();
    pixels.setPixelColor(0,pixels.Color(r,g,b));
    pixels.setBrightness(200);
    pixels.show();
}

void offpixels(){
    pixels.clear();
    pixels.show();
}
