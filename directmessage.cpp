#include "directmessage.h"

std::string DirectMessage::displayPost()
{
    return Post::displayPost() + "$" + std::to_string(recipient_) + "$";
}
