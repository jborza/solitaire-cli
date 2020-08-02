#include "loadLinkList.h"

void loadLinkList(
    struct linkList* headLinkList,
    char oneChar,
    int oneInt,
    struct linkList* tailLinkList
){
    // note no return; use by reference

    //oneLinkList->aChar     = oneChar;
    (* headLinkList).aChar  = oneChar;
    headLinkList->anInt     = oneInt;
    headLinkList->aLinkList = tailLinkList;

    // (* x). same as x-> 

}