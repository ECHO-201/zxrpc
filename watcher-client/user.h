#ifndef USER_H
#define USER_H

#include <QString>

class User
{
public:
    User();

    void setId(QString id);
    void setName(QString name);
    void setPhone(QString phone);

    QString getId();
    QString getName();
    QString getPhone();

private:
    QString id;
    QString name;
    QString phone;
};

#endif // USER_H
