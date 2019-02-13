#ifndef CUBE_H
#define CUBE_H

#include <algorithm>

class Cube {
public:
    int x, y, t;
    int w, h, l;

    Cube(void) {
        this->x = -1;
        this->y = -1;
        this->t = -1;

        this->w = -1;
        this->h = -1;
        this->l = -1;
    }
    Cube(int x, int y, int t, int width, int height, int length) {
        this->x = x;
        this->y = y;
        this->t = t;
        this->w = width;
        this->h = height;
        this->l = length;
    }
    Cube(const Cube& rhs) {
        this->x = rhs.x;
        this->y = rhs.y;
        this->t = rhs.t;
        this->w = rhs.w;
        this->h = rhs.h;
        this->l = rhs.l;
    }

    bool isValid() {
        if (this->x != -1 && this->y != -1 && this->t != -1 && this->w > 0 &&
        this->h > 0 && this->l > 0) {
            return true;
        }
        return false;
    }

    void setCube(int x, int y, int t, int w, int h, int l) {
        this->x = x;
        this->y = y;
        this->t = t;
        this->w = w;
        this->h = h;
        this->l = l;
    }

    Cube& operator = (const Cube& rhs) {
        if (this != &rhs) {
            this->x = rhs.x;
            this->y = rhs.y;
            this->t = rhs.t;
            this->w = rhs.w;
            this->h = rhs.h;
            this->l = rhs.l;
        }
        return *this;
    }

    ~Cube(void) {}
};

static inline bool operator == (const Cube& a, const Cube& b) {
    return a.x == b.x && a.y == b.y && a.t == b.t && a.w == b.w &&
        a.h == b.h && a.l == b.l;
}

static inline bool operator != (const Cube& a, const Cube& b) {
    return a.x != b.x || a.y != b.y || a.t != b.t || a.w != b.w ||
        a.h != b.h || a.l != b.l;
}

static inline Cube& operator&= (Cube& a, const Cube& b) {
    int x1 = std::max(a.x, b.x);
    int y1 = std::max(a.y, b.y);
    int t1 = std::max(a.t, b.t);
    a.w = std::min(a.x + a.w, b.x + b.w) - x1;
    a.h = std::min(a.y + a.h, b.y + b.h) - y1;
    a.l = std::min(a.t + a.l, b.t + b.l) - t1;
    a.x = x1;
    a.y = y1;
    a.t = t1;
    if (a.w <= 0 || a.h <= 0 || a.l <= 0)
        a = Cube();
    return a;
}

static inline Cube operator & (const Cube& a, const Cube& b) {
    Cube c = a;
    return c &= b;
}

#endif  // CUBE_H
