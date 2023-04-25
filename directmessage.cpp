#include "directmessage.h"
#include <iostream>

//pre: none
//post: return string of message, likes, and list of recipients
std::string DirectMessage::displayPost()
{
    return Post::displayPost() + "$" + std::to_string(recipient_) + "$";
}
