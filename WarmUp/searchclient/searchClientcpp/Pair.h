#ifndef PAIR_H
#define PAIR_H
//#include "centralHeader.h"
template <typename L, typename R> class Pair
{
    private:
      L left;
      R right;

    public:
      Pair(L left, R right):
      <typename L> L getLeft();
      <typename R> R getRight();
};

#endif
