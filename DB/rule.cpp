#include "rule.h"

Rule::Rule(QObject *parent) : QObject(parent)
{

}

Rule *Rule::Create(QObject *parent)
{
    auto rule = new Rule(parent);
    rule->setID(INVALID_ID);
    return rule;
}
