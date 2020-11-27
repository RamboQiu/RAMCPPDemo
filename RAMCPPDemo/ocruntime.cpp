//
//  ocruntime.cpp
//  RAMCPPDemo
//
//  Created by rambo on 2020/11/26.
//

#include "ocruntime.hpp"

typedef void (*IMP)(void /* id, SEL, ... */ );
struct objc_class;
struct objc_object;

typedef struct objc_class *Class;

struct Test {
    int a;
    long b;
    void test();
    void test2();
};

union isa_t {
    Class cls;
    unsigned long bits;
    struct {
        unsigned long nonpointer        : 1;
        unsigned long has_assoc         : 1;
        unsigned long has_cxx_dtor      : 1;
        unsigned long shiftcls          : 33;
        unsigned long magic             : 6;
        unsigned long weakly_referenced : 1;
        unsigned long deallocating      : 1;
        unsigned long has_sidetable_rc  : 1;
        unsigned long extra_rc          : 19;
    };
};

struct bucket_t {
    private:
        // IMP-first is better for arm64e ptrauth and no worse for arm64.
        // SEL-first is better for armv7* and i386 and x86_64.
    #if __arm64__
        MethodCacheIMP _imp;
        cache_key_t _key;
    #else
        unsigned long _key;
        IMP _imp;
    #endif
};
struct cache_t {
    struct bucket_t *_buckets;
    unsigned int _mask;
    unsigned int _occupied;
};

struct class_data_bits_t {
    unsigned long bits;
};

struct objc_object {
    // isa结构体
    isa_t isa;
};

struct objc_class : objc_object {
    // ISA占8位
    // Class ISA;
    // superclass占8位
    Class superclass;
    // 缓存的是指针和vtable,目的是加速方法的调用  cache占16位
    cache_t cache;             // formerly cache pointer and vtable
    // class_data_bits_t 相当于是class_rw_t 指针加上rr/alloc标志  占8位
    class_data_bits_t bits;    // class_rw_t * plus custom rr/alloc flags
};

void ocruntimetest() {
    printf("%lu\n", sizeof(objc_class));//共占 40
}
