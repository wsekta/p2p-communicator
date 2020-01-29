//
// Created by wojtek on 26.01.2020.
//


#include "include/Controller.hpp"
#include <QDebug>

Controller::Controller() {
    server = std::make_unique<Server>();
    database = std::make_unique<Database>();
    connect(server.get(), SIGNAL(newConnection(QTcpSocket * )), this, SLOT(onNewConnection(QTcpSocket * )));
}

void Controller::acceptConnection(qint8 idx) {
    connect(conversations[idx].get(), SIGNAL(newMessage(const QString &)),
            this, SLOT(onNewMessage(const QString &)));
}

void Controller::onNewConnection(QTcpSocket *socket) {
        currentConversation = std::make_shared<Conversation>("name", socket);
        conversations.push_front(currentConversation);        
        emit newPendingConnection(socket->peerAddress().toString().mid(7), QString::number(socket->peerPort()), "name");
}

void Controller::sendMessage(const QString &str) {
    if (currentConversation != nullptr)
        currentConversation->sendMessage(str);
}

void Controller::createNewConnection(QString name, const QString &ip, qint16 port)
{
    currentConversation = std::make_shared<Conversation>(name, ip, port);
    conversations.push_front(currentConversation);
    connect(currentConversation.get(), SIGNAL(newMessage(const QString &)),
            this, SLOT(onNewMessage(const QString &)));
    emit newConnection(ip, QString::number(port), name);
}

void Controller::onNewMessage(const QString &str) {
    lastMessage = str;
    emit newMessage();
}

const QString &Controller::getMessage() {
    return lastMessage;
}