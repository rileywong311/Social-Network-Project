#ifndef SOCIALNETWORKUI_H
#define SOCIALNETWORKUI_H

#include <QMainWindow>
#include "user.h"
#include "network.h"
#include <stack>

QT_BEGIN_NAMESPACE
namespace Ui { class SocialNetworkUI; }
QT_END_NAMESPACE

class SocialNetworkUI : public QMainWindow
{
    Q_OBJECT

public:
    SocialNetworkUI(QWidget *parent = nullptr);
    ~SocialNetworkUI();

private:
    Ui::SocialNetworkUI *ui;
    User *loggedInUser_;
    std::string displayUser_;
    Network net_;

    std::stack<std::string> breadcrumbs_;
    bool hasBreadcrumbs() const;
    void addBreadcrumb(std::string data);

private slots:
    void login();
    void friendClicked(int row, int col);
    void updatePage(std::string user);
    void addFriend();
    void returnHome();
    void shortestPath();
    void suggestFriend();
    void addFriendClicked(int row, int col);
    void undoBreadcrumb();
    void logOut();
};
#endif // SOCIALNETWORKUI_H
