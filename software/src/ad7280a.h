#ifndef AD7280A_H
#define AD7280A_H

struct ad7280a {
    SPIDriver *spid;
};

typedef struct ad7280a ad7280a_t;

#endif // AD7280A_H
