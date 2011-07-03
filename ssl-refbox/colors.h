#ifndef COLORS_H
#define COLORS_H

namespace SSLRefbox
{
    class Colors
    {
    public:
        Colors()
        {
        }
        virtual ~Colors()
        {
        }

        enum Color
        {
            BLACK,
            WHITE,
            YELLOW,
            BLUE,
            GREEN,
            RED,
            PINK,
            MAGENTA,
            ORANGE,
            WHITE_TRANSPARENT,
            GREY,
            FIELD,
            NONE
        } color;
    };
}

#endif //COLORS_H
