#ifndef POOL_H
#define POOL_H

#define POOL_SIZE_ELEMENTS 10

template <typename T, int SIZE>
class Pool
{
public:
    Pool(void);
    T *palloc(void);
    void free(T *p);
    float used(void);

private:
    int getIndexFromPointer(T *p);
    uint8_t index[POOL_SIZE_ELEMENTS] = {0};
    T poolData[POOL_SIZE_ELEMENTS] = {0};
};

#endif
