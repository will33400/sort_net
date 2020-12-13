#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QString>

class Connector
{
private:
    int m_ei;
    int m_eo;
    int m_pos;
public:
    Connector(int ei, int eo, int pos);

    int getEi() const;
    int getEo() const;
    int getPos() const;
    QString toString();
    static bool lessPositionThan(Connector conn_1, Connector conn_2);
    static bool lessDepuisThan(Connector conn_1, Connector conn_2);
    void setPos(int pos);
};

#endif // CONNECTOR_H
