#include <iostream>

#include "Widget.h"

Widget::Widget(int x, int y, int width, int height)
{
    std::cout << "Constructed Widget: (" << x << ", " << y << ") " << width << "x" << height << std::endl;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

int Widget::getX()
{
    return x;
}

int Widget::getY()
{
    return y;
}

int Widget::getWidth()
{
    return width;
}

int Widget::getHeight()
{
    return height;
}


void Widget::setTitle(std::string newTitle)
{
    this->title = newTitle;
}
