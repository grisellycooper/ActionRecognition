#ifndef CUBE_H
#define CUBE_H

#include <algorithm>

class Cube {
public:
    int x0, y0, t0;
    int w, h, l;

    Cube(void) {
        this->x0 = -1;
        this->y0 = -1;
        this->t0 = -1;
        this->w = -1;
        this->h = -1;
        this->l = -1;
    }
    Cube(int x, int y, int t, int width, int height, int length) {
        this->x0 = x;
        this->y0 = y;
        this->t0 = t;
        this->w = width;
        this->h = height;
        this->l = length;
    }
    Cube(const Cube& rhs) {
        this->x0 = rhs.x0;
        this->y0 = rhs.y0;
        this->t0 = rhs.t0;
        this->w = rhs.w;
        this->h = rhs.h;
        this->l = rhs.l;
    }

    bool isValid() {
        if (this->x0 != -1 && this->y0 != -1 && this->t0 != -1 && this->w > 0 &&
        this->h > 0 && this->l > 0) {
            return true;
        }
        return false;
    }

    void setCube(int x0, int y0, int t0, int w, int h, int l) {
        this->x0 = x0;
        this->y0 = y0;
        this->t0 = t0;
        this->w = w;
        this->h = h;
        this->l = l;
    }

    Cube& operator = (const Cube& rhs) {
        if (this != &rhs) {
            this->x0 = rhs.x0;
            this->y0 = rhs.y0;
            this->t0 = rhs.t0;
            this->w = rhs.w;
            this->h = rhs.h;
            this->l = rhs.l;
        }
        return *this;
    }

    ~Cube(void) {}
};

static inline bool operator == (const Cube& a, const Cube& b) {
    return a.x0 == b.x0 && a.y0 == b.y0 && a.t0 == b.t0 && a.w == b.w &&
        a.h == b.h && a.l == b.l;
}

static inline bool operator != (const Cube& a, const Cube& b) {
    return a.x0 != b.x0 || a.y0 != b.y0 || a.t0 != b.t0 || a.w != b.w ||
        a.h != b.h || a.l != b.l;
}

static inline Cube& operator&= (Cube& a, const Cube& b) {
    int x1 = std::max(a.x0, b.x0);
    int y1 = std::max(a.y0, b.y0);
    int t1 = std::max(a.t0, b.t0);
    a.w = std::min(a.x0 + a.w, b.x0 + b.w) - x1;
    a.h = std::min(a.y0 + a.h, b.y0 + b.h) - y1;
    a.l = std::min(a.t0 + a.l, b.t0 + b.l) - t1;
    a.x0 = x1;
    a.y0 = y1;
    a.t0 = t1;
    if (a.w <= 0 || a.h <= 0 || a.l <= 0)
        a = Cube();
    return a;
}

static inline Cube operator & (const Cube& a, const Cube& b) {
    Cube c = a;
    return c &= b;
}

#endif  // CUBE_H
