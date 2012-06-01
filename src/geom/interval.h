#ifndef INTERVAL_H
#define INTERVAL_H

struct Interval
{
    Interval()
    {
        min = 0;
        max = 0;
    }

    Interval(double a, double b)
    {
        this->min = fmin(a, b);
        this->max = fmax(a, b);
    }

    Interval join(const Interval &i) const
    {
        return Interval(fmin(min, i.min), fmax(max, i.max));
    }

    bool contains(double v) const
    {
        return v >= min && v <= max;
    }

    double size() const
    {
        return max - min;
    }

    Interval growFraction(double f) const
    {
        double change = f * size() / 2.0;
        return Interval(min - change, max + change);
    }

    double min;
    double max;
};

#endif // INTERVAL_H
