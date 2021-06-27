//
// Created by natha on 13/12/2020.
//

#include "Power.h"

Power::Power(){
    m_amount = 0.0;
}

Power::Power(double amount, PowerUnit powerUnit){
    if (powerUnit == kW){
        m_amount = amount;
    }else if (powerUnit == mW){
        m_amount = amount*1000;
    }else if (powerUnit == gW){
        m_amount = amount*1000000;
    }
}

std::string Power::asStrAutoUnit() const{
    if (m_amount >= 1000000){
        return asStr(gW);
    }else if(m_amount >= 1000){
        return asStr(mW);
    }else{
        return asStr(kW);
    }
}

std::string Power::asStr(PowerUnit powerUnit) const{
    // Create an output string stream
    std::ostringstream ss;
    // Set Fixed -Point Notation
    ss << std::fixed;
    // Set precision to 2 digits
    ss << std::setprecision(2);

    //Add double to stream
    if (powerUnit == kW){
        ss << m_amount;
    }else if (powerUnit == mW){
        ss << m_amount/1000;
    }else if (powerUnit == gW){
        ss << m_amount/1000000;
    }

    // Get string from output string stream
    return ss.str();
}
double Power::asDouble(PowerUnit powerUnit) const{
    if (powerUnit == kW){
        return m_amount;
    }else if (powerUnit == mW){
        return m_amount/1000;
    }else if (powerUnit == gW){
        return m_amount/1000000;
    }
}

Power Power::operator+(const Power &p){
    return Power(m_amount+p.m_amount);
}
Power Power::operator-(const Power &p){
    return Power(m_amount-p.m_amount);
}
Power Power::operator+(double amount){
    return Power(m_amount+amount);
}
Power Power::operator-(double amount){
    return Power(m_amount-amount);
}
Power Power::operator*(double fact){
    return Power(m_amount*fact);
}
Power Power::operator/(double fact){
    return Power(m_amount/fact);
}

void Power::operator-=(const Power &p){
    m_amount-=p.m_amount;
}
void Power::operator+=(const Power &p){
    m_amount+=p.m_amount;
}
void Power::operator*=(double fact){
    m_amount*=fact;
}
void Power::operator/=(double fact){
    m_amount/=fact;
}

bool Power::operator<(const Power &p){
    return m_amount<p.m_amount;
}
bool Power::operator<=(const Power &p){
    return m_amount<=p.m_amount;
}
bool Power::operator>(const Power &p){
    return m_amount>p.m_amount;
}
bool Power::operator>=(const Power &p){
    return m_amount>=p.m_amount;
}
bool Power::operator==(const Power &p){
    return m_amount==p.m_amount;
}

std::ostream& operator<<(std::ostream &oss, const Power &p){
    oss << p.asStrAutoUnit();
    return oss;
}
