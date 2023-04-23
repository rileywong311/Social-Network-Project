#ifndef DIRECTMESSAGE_H
#define DIRECTMESSAGE_H

#include "post.h"

class DirectMessage : public Post
{
public:
    // pre: none
    // post: construct default DM
    DirectMessage(){}

    // pre: none
    // post: DM constructed
    DirectMessage(int id, int author, int likes, std::string message, int recipient) : Post(id, author, likes, message)
    {
        recipient_ = recipient;
    }

    //pre: none
    //post: return string of message, likes, and list of recipients
    std::string displayPost();

    // pre: none
    // post: return recipient
    int recipient() const { return recipient_; }

private:
    int recipient_;
};

#endif // DIRECTMESSAGE_H
