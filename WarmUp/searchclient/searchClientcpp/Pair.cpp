#include "Pair.h"
//#include "centralHeader.h"

/**
 * Source: https://stackoverflow.com/questions/521171/a-java-collection-of-value-pairs-tuples
 * @param <L>
 * @param <R>
 */
namespace pair {

  Pair(L left, R right)
  {
      this.left = left;
      this.right = right;
  }

  <typename L> L getLeft() { return left; }
  <typename R> R getRight() { return right; }

}
