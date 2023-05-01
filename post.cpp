#include "post.h"
#include <iostream>

// pre: none
// post: post constructed
Post::Post(int id, int author, int likes, std::string message)
{
    id_ = id;
    author_ = author;
    likes_ = likes;
    message_ = message;
}

//pre: none
//post: returns message followed by likes on a new line
std::string Post::displayPost()
{
    return message_ + "\n" + std::to_string(likes_) + " Likes";
}
