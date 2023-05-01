#include "socialnetworkui.h"
#include "ui_socialnetworkui.h"

void show_login(Ui::SocialNetworkUI *ui);
void hide_login(Ui::SocialNetworkUI *ui);
void show_user_page(Ui::SocialNetworkUI *ui);
void hide_user_page(Ui::SocialNetworkUI *ui);

SocialNetworkUI::SocialNetworkUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SocialNetworkUI)
{
    ui->setupUi(this);

    // set private vars
    loggedInUser_ = nullptr;
    net_ = Network();
    net_.read_friends("C://cygwin64//home//yelir//SocialNetwork//SocialNetworkUI//SN-2.txt");
    net_.read_posts("C://cygwin64//home//yelir//SocialNetwork//SocialNetworkUI//posts-1.txt");

    // login page
    show_login(ui);
    connect(ui->loginButton, &QPushButton::clicked, this, &SocialNetworkUI::login);

    // user page
    hide_user_page(ui);
    ui->friendsTable->setColumnCount(1);
    ui->addFriendButton->setVisible(false);
    connect(ui->friendsTable, &QTableWidget::cellClicked, this, &SocialNetworkUI::friendClicked);
    connect(ui->addFriendButton, &QPushButton::clicked, this, &SocialNetworkUI::addFriend);
}

SocialNetworkUI::~SocialNetworkUI()
{
    delete ui;
    loggedInUser_ = nullptr;
}

void SocialNetworkUI::login()
{
    int id = net_.get_id(ui->loginTextEdit->toPlainText().toStdString());
    if(id == -1)
    {
        ui->loginFailLabel->setVisible(true);
    }
    else
    {
        hide_login(ui);

        loggedInUser_ = net_.get_user(id);
        updatePage(loggedInUser_->name());
        show_user_page(ui);

    }
    ui->loginTextEdit->setPlainText("");
}


void SocialNetworkUI::friendClicked(int row, int col)
{
    ui->friendsTable->setCurrentCell(row,  col);
    QString qs_user_name = ui->friendsTable->currentItem()->text();
    //QString qs_user_name = ui->friendsTable->itemAt(row, col)->data(0).toString();
    std::string user_name = qs_user_name.toStdString();
    updatePage(user_name);
}

void SocialNetworkUI::updatePage(std::string user)
{
    displayUser_ = user;
    if(user == loggedInUser_->name())
    {
        ui->usernameLabel->setText(QString::fromStdString("My Profile"));
        ui->addFriendButton->setVisible(false);
    }
    else
    {
        ui->usernameLabel->setText(QString::fromStdString(user + "'s Profile"));
        if(std::find(loggedInUser_->friends()->begin(), loggedInUser_->friends()->end(), net_.get_id(user)) == loggedInUser_->friends()->end())
            ui->addFriendButton->setVisible(true);
        else
            ui->addFriendButton->setVisible(false);
    }

    std::vector<std::size_t> *friends = net_.get_user(net_.get_id(user))->friends();

    ui->friendsTable->clear();
    ui->friendsTable->setRowCount(friends->size());
    for(size_t i=0; i<friends->size(); ++i)
    {
        std::string name = net_.get_user((*friends)[i])->name();
        QString qs_name = QString::fromStdString(name);
        QTableWidgetItem *qtwi = new QTableWidgetItem(qs_name);
        // std::cout<<"DEBUG: "<<name<<std::endl;
        qtwi->setText(qs_name);
        qtwi->setData(0, qs_name);
        ui->friendsTable->setItem(i, 0, qtwi);
    }

    std::string posts = net_.displayPosts(user, 5);
    if(posts == "")
        posts = "No Posts";
    ui->postsLabel->setText(QString::fromStdString(posts));
}

void SocialNetworkUI::addFriend()
{
    net_.add_connection(displayUser_, loggedInUser_->name());
    updatePage(displayUser_);
}


void show_login(Ui::SocialNetworkUI *ui)
{
    ui->loginTextEdit->setVisible(true);
    ui->loginButton->setVisible(true);
    ui->loginLabel->setVisible(true);
    ui->loginFailLabel->setVisible(false);
}

void hide_login(Ui::SocialNetworkUI *ui)
{
    ui->loginTextEdit->setVisible(false);
    ui->loginButton->setVisible(false);
    ui->loginLabel->setVisible(false);
    ui->loginFailLabel->setVisible(false);
}

void show_user_page(Ui::SocialNetworkUI *ui)
{
    ui->friendsTable->setVisible(true);
    ui->friendsLabel->setVisible(true);
    ui->postsLabel->setVisible(true);
    ui->usernameLabel->setVisible(true);
    ui->pathButton->setVisible(true);
    ui->suggestButton->setVisible(true);
}

void hide_user_page(Ui::SocialNetworkUI *ui)
{
    ui->friendsLabel->setVisible(false);
    ui->friendsTable->setVisible(false);
    ui->postsLabel->setVisible(false);
    ui->usernameLabel->setVisible(false);
    ui->pathButton->setVisible(false);
    ui->suggestButton->setVisible(false);
}
