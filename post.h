#ifndef POST_H
#define POST_H

#include <string>

class Post
{
public:

    // pre: none
    // post: construct default post
    Post(){}

    // pre: none
    // post: post constructed
    Post(int id, int author, int likes, std::string message);

    //pre: none
    //post: returns message followed by likes on a new line
    virtual std::string displayPost();

    //pre: none
    //post: returns id
    int id() const { return id_; }

    //pre: none
    //post: returns author
    int author() const { return author_; }

    //pre: none
    //post: returns likes
    int likes() const { return likes_; }

    //pre: none
    //post: returns message
    std::string message() const { return message_; }

private:

    int id_, author_, likes_;
    std::string message_;

};


#endif // POST_H
