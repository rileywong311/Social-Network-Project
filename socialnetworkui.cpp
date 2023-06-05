 #include "socialnetworkui.h"
#include "ui_socialnetworkui.h"


void update_friends_table(Ui::SocialNetworkUI *ui);
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
    ui->profileButton->setVisible(false);
    ui->pathButton->setVisible(false);
    ui->suggestButton->setVisible(false);
    ui->friendsLabel->setVisible(false);
    ui->friendTextEdit->setVisible(false);
    ui->suggestFriendWidget->setColumnCount(1);
    ui->suggestFriendWidget->setVisible(false);
    ui->friendInfoLabel->setText(QString::fromStdString(""));
    ui->backButton->setVisible(false);
    connect(ui->friendsTable, &QTableWidget::cellClicked, this, &SocialNetworkUI::friendClicked);
    connect(ui->addFriendButton, &QPushButton::clicked, this, &SocialNetworkUI::addFriend);
    connect(ui->profileButton, &QPushButton::clicked, this, &SocialNetworkUI::returnHome);
    connect(ui->suggestButton, &QPushButton::clicked, this, &SocialNetworkUI::suggestFriend);
    connect(ui->pathButton, &QPushButton::clicked, this, &SocialNetworkUI::shortestPath);
    connect(ui->suggestFriendWidget, &QTableWidget::cellClicked, this, &SocialNetworkUI::addFriendClicked);
    connect(ui->backButton, &QPushButton::clicked, this, &SocialNetworkUI::undoBreadcrumb);
    connect(ui->logOutButton, &QPushButton::clicked, this, &SocialNetworkUI::logOut);
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
    addBreadcrumb(user_name);
    updatePage(user_name);
}

void SocialNetworkUI::updatePage(std::string user)
{
    if(hasBreadcrumbs())
        ui->backButton->setVisible(true);
    else
        ui->backButton->setVisible(false);
    displayUser_ = user;
    ui->suggestFriendWidget->setVisible(false);
    if(user == loggedInUser_->name())
    {
        ui->usernameLabel->setText(QString::fromStdString("My Profile"));
        ui->addFriendButton->setVisible(false);
        ui->profileButton->setVisible(false);
        ui->pathButton->setVisible(true);
        ui->suggestButton->setVisible(true);
        ui->friendTextEdit->setVisible(true);
    }
    else
    {
        ui->usernameLabel->setText(QString::fromStdString(user + "'s Profile"));
        ui->profileButton->setVisible(true);
        ui->pathButton->setVisible(false);
        ui->suggestButton->setVisible(false);
        ui->friendTextEdit->setVisible(false);
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

void SocialNetworkUI::returnHome()
{
    addBreadcrumb(loggedInUser_->name());
    updatePage(loggedInUser_->name());
}

void SocialNetworkUI::shortestPath()
{
    ui->friendInfoLabel->setVisible(true);
    ui->suggestFriendWidget->setVisible(false);
    std::string path;
    int to = net_.get_id(ui->friendTextEdit->toPlainText().toStdString());
    if(to == -1)
    {
        path = "User \"" + ui->friendTextEdit->toPlainText().toStdString() + "\" Does Not Exist!";
    }
    else
    {
        int from = loggedInUser_->id();
        std::vector<int> shortest = net_.shortest_path(from, to);

        if(shortest.empty())
            path = "No Path Exists!";
        else
        {
            path = "Distance: ";
            path += std::to_string(net_.get_user(shortest[shortest.size()-1])->depth) + '\n';
            for(std::size_t i = 0; i < shortest.size() - 1; ++i)
                path += net_.get_user(shortest[i])->name() + " -> ";
            path += net_.get_user(shortest[shortest.size()-1])->name();
        }
    }
    ui->friendInfoLabel->setText(QString::fromStdString(path));
}

void SocialNetworkUI::suggestFriend()
{

    int score;
    std::vector<int> suggest_result = net_.suggest_friends(net_.get_id(loggedInUser_->name()), score);

    if(score == -1)
    {
        ui->friendInfoLabel->setVisible(true);
        ui->suggestFriendWidget->setVisible(false);
        ui->friendInfoLabel->setText(QString::fromStdString("No Suggestion!"));
        return;
    }

    ui->friendInfoLabel->setVisible(false);
    ui->suggestFriendWidget->setVisible(true);

    ui->suggestFriendWidget->clear();
    ui->suggestFriendWidget->setRowCount(suggest_result.size());
    for(size_t i=0; i<suggest_result.size(); ++i)
    {
        std::string name = "Add Friend: ";
        name += net_.get_user(suggest_result[i])->name();
        name += " (" + std::to_string(score) + "*)";
        QString qs_name = QString::fromStdString(name);
        QTableWidgetItem *qtwi = new QTableWidgetItem(qs_name);
        qtwi->setData(1, QString::fromStdString(net_.get_user(suggest_result[i])->name()));
        ui->suggestFriendWidget->setItem(i, 0, qtwi);
    }
}

void SocialNetworkUI::addFriendClicked(int row, int col)
{
    ui->suggestFriendWidget->setCurrentCell(row, col);
    QString qs_user_name = ui->suggestFriendWidget->itemAt(row, col)->data(1).toString();
    std::string user_name = qs_user_name.toStdString();
    net_.add_connection(user_name, loggedInUser_->name());
    updatePage(loggedInUser_->name());

    std::vector<std::size_t> *friends = net_.get_user(net_.get_id(loggedInUser_->name()))->friends();

    ui->friendsTable->clear();
    ui->friendsTable->setRowCount(friends->size());
    for(size_t i=0; i<friends->size(); ++i)
    {
        std::string name = net_.get_user((*friends)[i])->name();
        QString qs_name = QString::fromStdString(name);
        QTableWidgetItem *qtwi = new QTableWidgetItem(qs_name);
        ui->friendsTable->setItem(i, 0, qtwi);
    }

    suggestFriend();
}

void SocialNetworkUI::addBreadcrumb(std::string data)
{
    breadcrumbs_.push(data);
}

bool SocialNetworkUI::hasBreadcrumbs() const
{
    return !breadcrumbs_.empty();
}

void SocialNetworkUI::undoBreadcrumb()
{
    if(hasBreadcrumbs())
        breadcrumbs_.pop();
    if(hasBreadcrumbs())
        updatePage(breadcrumbs_.top());
    else
        updatePage(loggedInUser_->name());
}

void SocialNetworkUI::logOut()
{
    while(hasBreadcrumbs())
        breadcrumbs_.pop();
    loggedInUser_ = nullptr;
    displayUser_ = std::string();
    hide_user_page(ui);
    ui->addFriendButton->setVisible(false);
    ui->profileButton->setVisible(false);
    ui->pathButton->setVisible(false);
    ui->suggestButton->setVisible(false);
    ui->friendsLabel->setVisible(false);
    ui->friendTextEdit->setVisible(false);
    ui->suggestFriendWidget->setVisible(false);
    ui->backButton->setVisible(false);
    show_login(ui);
}

void show_login(Ui::SocialNetworkUI *ui)
{
    ui->loginTextEdit->setVisible(true);
    ui->loginButton->setVisible(true);
    ui->loginLabel->setVisible(true);
    ui->loginFailLabel->setVisible(false);
    ui->logOutButton->setVisible(false);
}

void hide_login(Ui::SocialNetworkUI *ui)
{
    ui->loginTextEdit->setVisible(false);
    ui->loginButton->setVisible(false);
    ui->loginLabel->setVisible(false);
    ui->loginFailLabel->setVisible(false);
    ui->logOutButton->setVisible(true);
}

void show_user_page(Ui::SocialNetworkUI *ui)
{
    ui->friendsTable->setVisible(true);
    ui->friendsLabel->setVisible(true);
    ui->postsLabel->setVisible(true);
    ui->usernameLabel->setVisible(true);
}

void hide_user_page(Ui::SocialNetworkUI *ui)
{
    ui->friendsLabel->setVisible(false);
    ui->friendsTable->setVisible(false);
    ui->postsLabel->setVisible(false);
    ui->usernameLabel->setVisible(false);
}

