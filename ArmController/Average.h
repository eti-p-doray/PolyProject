#ifndef AVERAGE_H
#define AVERAGE_H

class Signal {
public:
    Signal() {sum = 0.0; n = 0.0;}
    void set(double value) {sum += value; n++; double delta = value-sum/n; M2+=delta*(value-sum/n);}
    double getAverage() {return sum/n;}
    double getVariance() {return M2/(n-1);}
    
private:
    double sum;
    double n;
    double M2;
};

#endif
