#include "user.h"

User::User()
{

}

void User::setId(QString id){
    this->id = id;
}

void User::setName(QString name){
    this->name = name;
}

void User::setPhone(QString phone){
    this->phone = phone;
}

QString User::getId(){
    return id;
}

QString User::getName(){
    return name;
}

QString User::getPhone(){
    return phone;
}
