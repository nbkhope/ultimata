#ifndef __WIDGET_H__
#define __WIDGET_H__

class Widget
{
public:
    Widget(int x, int y, int width, int height);

    int getX();
    int getY();
    int getWidth();
    int getHeight();

    void setTitle(std::string newTitle);
    
    void setVisible(bool visible);
    bool isVisible() const;

private:
    int x, y;
    int width, height;
    std::string title;
    bool visible;
};

#endif
