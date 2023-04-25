#include "directmessage.h"
#include <iostream>

std::string DirectMessage::displayPost()
{
    return Post::displayPost() + "$" + std::to_string(recipient_) + "$";
}
