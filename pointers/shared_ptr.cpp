#include <iostream>

/*
  A very rudimentary shared pointer class.

  g++ shared_ptr.cpp -o shared_ptr.o && ./shared_ptr.o
 */
template <typename T>
class SharedPtr {
public:
  SharedPtr() {
    count = new int(1);
    std::cout << "run no val ctor" << std::endl;
  }
  
  SharedPtr(const T* val) {
    count = new int(1);
    this->val = val;
    std::cout << "val in new ctor: " << *val << std::endl;
    std::cout << "count in new ctor: " << *count << std::endl;
  }

  SharedPtr(const SharedPtr& other) {
    count = other.count;
    (*count)++;
    val = other.val;
    std::cout << "val in copy ctor: " << *val << std::endl;
    std::cout << "count in copy ctor: " << *count << std::endl;
  }

  T operator*() {
    return *val;
  }

  operator bool() const {
    return (val != nullptr);
  }
  
  ~SharedPtr() {
    (*count)--;
    if (*count == 0) {
      if (val) {
        // may not have val if used no-arg constructor.
        delete val;
      }
      delete count;
    }

    if (val) {
      std::cout << "val in dtor: " << *val << std::endl;
    }
    std::cout << "count in dtor: " << *count << std::endl;
  };

private:
  const T* val = nullptr;
  int* count;
};

void foo(SharedPtr<int> ptr) {
  std::cout << "shared ptr val in foo: " << *ptr << std::endl;
}

int main() {
  std::cout << "creating my_val..." << std::endl;
  SharedPtr<int> my_val(new int(15));
  std::cout << "...created my_val." << std::endl;

  std::cout << "val before foo: " << *my_val << std::endl;  
  foo(my_val);
  std::cout << "val after foo: " << *my_val << std::endl;
  std::cout << "my_val is " << (my_val ? "not null" : "null") << std::endl;
  std::cout << ".........................." << std::endl;
  std::cout << "creating another..." << std::endl;
  SharedPtr<int> another;
  std::cout << "...created another." << std::endl;
  std::cout << "another is " << (another ? "not null" : "null") << std::endl;

  /*
    creating my_val...
    val in new ctor: 15
    count in new ctor: 1
    ...created my_val.
    val before foo: 15
    val in copy ctor: 15
    count in copy ctor: 2
    shared ptr val in foo: 15
    val in dtor: 15
    count in dtor: 1
    val after foo: 15
    my_val is not null
    ..........................
    creating another...
    run no val ctor
    ...created another.
    another is null
    count in dtor: 0
    val in dtor: 1968984768
    count in dtor: 1968984704
   */
}
