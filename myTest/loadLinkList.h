#ifndef LOADLINKLIST_H
#define LOADLINKLIST_H
struct linkList {
    char aChar;
    int anInt;
    struct linkList * aLinkList;
};

void loadLinkList(
    struct linkList* headLinkList,
    char oneChar,
    int oneInt,
    struct linkList* tailLinkList
);
#endif