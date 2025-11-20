#ifndef button_list_h
#define button_list_h
// THE BUTTON FILE
// button vars
struct pos1
{
    int x;
    int y;
};
struct pos2
{
    int x;
    int y;
};
struct buttons_list
{
    char *name;
    char *displayText;

    int pos1X;
    int pos1Y;

    int pos2X;
    int pos2Y;

    bool pressed;
    char *command;
};

#endif