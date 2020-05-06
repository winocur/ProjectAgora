struct Color {
    int r;
    int g;
    int b;
    int a;
};

Color operator+ (Color color1, Color color2) {

    Color color = {
        color1.r + color2.r,
        color1.g + color2.g,
        color1.b + color2.b,
        color1.a + color2.a,
    };

    if(color.r < 0) color.r = 0;
    if(color.g < 0) color.g = 0;
    if(color.b < 0) color.b = 0;
    if(color.a < 0) color.a = 0;

    return color;
}