#ifndef SOCIALNETWORKUI_H
#define SOCIALNETWORKUI_H

#include <QMainWindow>
#include "user.h"
#include "network.h"

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

private slots:
    void login();
    void friendClicked(int row, int col);
    void updatePage(std::string user);
    void addFriend();

};
#endif // SOCIALNETWORKUI_H
