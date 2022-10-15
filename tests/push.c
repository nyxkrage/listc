#define LIST_TYPE int
#include "../list.h"
#undef LIST_TYPE

#include "test.h"

int main(void) {
    void* expected_data = malloc(sizeof(int) * 5);
    for (int i = 0; i < 5; i++) {
        *(int*)(expected_data + sizeof(int) * i) = i;
    }
    List expected = (List){
        .data = expected_data,
        .length = 5,
        .capacity = 8,
    };

    List actual = new_list(int)();
    for (int i = 0; i < 5; i++) {
        push_list(int)(&actual,i);
    }
    ASSERT(
        get_list(int)(&expected, 4),
        get_list(int)(&actual, 4),
        "Expected %d but got %d"
    );
}
