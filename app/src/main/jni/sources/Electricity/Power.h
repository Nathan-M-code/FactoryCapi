//
// Created by natha on 13/12/2020.
//

#ifndef FACTORYCAPICLION_POWER_H
#define FACTORYCAPICLION_POWER_H

#include "../utile.h"

class Power {
public:
    Power();
    Power(double amount, PowerUnit powerUnit=kW);

    std::string asStr(PowerUnit powerUnit) const;
    std::string asStrAutoUnit() const;
    double asDouble(PowerUnit powerUnit) const;

    Power operator+(const Power &p);
    Power operator-(const Power &p);
    Power operator+(double amount);
    Power operator-(double amount);
    Power operator*(double fact);
    Power operator/(double fact);

    void operator-=(const Power &p);
    void operator+=(const Power &p);
    void operator*=(double fact);
    void operator/=(double fact);

    bool operator<(const Power &p);
    bool operator<=(const Power &p);
    bool operator>(const Power &p);
    bool operator>=(const Power &p);
    bool operator==(const Power &p);
private:
    double m_amount; //kW by default
};

std::ostream& operator<<(std::ostream &oss, const Power &p);





#endif //FACTORYCAPICLION_POWER_H
