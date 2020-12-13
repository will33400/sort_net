#ifndef SORTNET_H
#define SORTNET_H

#include <QList>
#include "connector.h"


class Sortnet : public QList <Connector *>
{
private:
    int m_nb_wire = 0;
    int m_pos_max = 0;
    QList <int> m_val_list;
    int m_val_ind = 0;
public:
    Sortnet(int nb);
    void AppendConnector(int ei, int eo, int pos);
    void AppendConnector(int ei, int eo);
    void AppendConnector(Connector *c);
    QString toString();
    int GetPosMax();
    Connector getConnector(int pos);
    void inject(QList<int> values, int size);
    int pop(int index);
    void replace(int index1, int index2);
    bool Zero_oneTest();
    void parallel();
    void run();
    bool sorted();
    int getVal_ind() const;
};

#endif // SORTNET_H
