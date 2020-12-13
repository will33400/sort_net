#include "sortnet.h"
#include <QTextStream>
#include <math.h>
#include <QDebug>

int Sortnet::getVal_ind() const
{
    return m_val_ind;
}

Sortnet::Sortnet(int nb): m_nb_wire(nb)
{

}

void Sortnet::AppendConnector(int ei, int eo, int pos)
{
    if(m_pos_max < m_nb_wire) {
        Connector *c = new Connector(ei, eo, pos);
        append(c);
        m_pos_max++;
        std::sort(*begin(), *end(), Connector::lessPositionThan);
    }
}

void Sortnet::AppendConnector(int ei, int eo)
{
    Connector *c;
    if(m_pos_max < m_nb_wire) {
        if(ei < eo)
            c = new Connector(ei, eo, m_pos_max+1);
        else
            c = new Connector(eo, ei, m_pos_max+1);

        append(c);
        m_pos_max++;
    }
}

void Sortnet::AppendConnector(Connector *c)
{
    if(m_pos_max < m_nb_wire) {
        append(c);
        m_pos_max++;
        std::sort(*begin(), *end(), Connector::lessPositionThan);
    }
}

QString Sortnet::toString()
{
    QString res;
    QTextStream buf(&res);

    for(int i = 0; i < m_pos_max; i++)
        buf<<"Connector num : "<<i<<", "<<at(i)->toString()<<endl;
    buf<<"list state : ";
    for(int i = 0; i < m_val_ind; i++)
        buf<<m_val_list.at(i);
    buf<<endl;
    return res;
}

int Sortnet::GetPosMax()
{
    return m_pos_max;
}

Connector Sortnet::getConnector(int pos)
{
    return *at(pos);
}

void Sortnet::inject(QList<int> values, int size)
{
    m_val_list = values;
    m_val_ind = size;
}

int Sortnet::pop(int index)
{
    return m_val_list.at(index);
}

void Sortnet::replace(int index1, int index2)
{
    m_val_list.replace(index1, index2);
}

bool Sortnet::Zero_oneTest()
{
    /*
    bool toggle = m_val_list.at(0)&1;
    for(int i = 0; i < m_pos_max; i++) {
        if((m_val_list.at(i)&1) != toggle)
            return false;
        toggle ^= true;
    }
    return true;*//*
    QList<int> v_input;
    for(int i = 0; i < pow(2, m_nb_wire); i++) {
        int v = i;
        for(int j = 0; j < m_nb_wire; j++) {
            v_input.prepend(v/2);
            v %= 2;
        }
    }*/
    return true;
}

void Sortnet::parallel()
{
    for(int i = 0; i < m_pos_max-1; i++) {
        if(
            (
                at(i)->getEi() < at(i+1)->getEi() &&
                at(i)->getEo() < at(i+1)->getEo() &&
                at(i)->getEi() < at(i+1)->getEo() &&
                at(i)->getEo() < at(i+1)->getEi()
            ) ||
            (
                at(i)->getEi() > at(i+1)->getEi() &&
                at(i)->getEo() > at(i+1)->getEo() &&
                at(i)->getEi() > at(i+1)->getEo() &&
                at(i)->getEo() > at(i+1)->getEi()
            )
        ) {

            at(i+1)->setPos(i+1);

            for(int j = i+2; j < m_pos_max; j++)
                at(j)->setPos(at(i+1)->getPos()+1);
        }
    }
}

void Sortnet::run()
{
    for(int i = 0; i < m_pos_max; i++) {
        if(m_val_list.at(at(i)->getEi()) > m_val_list.at(at(i)->getEo())) {
            int temp = m_val_list.at(at(i)->getEi());
            m_val_list.replace(at(i)->getEi(), m_val_list.at(at(i)->getEo()));
            m_val_list.replace(at(i)->getEo(), temp);
        }
    }
    parallel();
}

bool Sortnet::sorted()
{

    for(int i = 0; i < m_val_ind - 1; i++) {
        if(m_val_list.at(i) > m_val_list.at(i+1))
            return false;
    }
    return true;
}
