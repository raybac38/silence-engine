#include "../../src/utils/sparse_set.h"
#include "sparse_set_test.h"
#include <string>

bool SparseSetTest::test()
{
    return test_1() && test_2();
}

bool SparseSetTest::test_1()
{
    SparseSet<std::string> set = SparseSet<std::string>();

    std::string string1 = "hey";
    std::string string2 = "coucou";

    size_t index1 = set.insert(string1);
    size_t index2 = set.insert(string2);

    std::vector<std::string> vec = set.data();

    bool success1 = vec.at(index1) == string1;
    if (!success1)
    {
        fprintf(stderr, "Test 1 failed : vec.at(0) != string1\n");
    }
    bool succes2 = vec.at(index2) == string2;
    if (!succes2)
    {
        fprintf(stderr, "Test 1 failed : vec.at(1) != string2\n");
    }

    return success1 && succes2;
}

bool SparseSetTest::test_2()
{
    SparseSet<std::string> set = SparseSet<std::string>();

    std::string string1 = "hey";
    std::string string2 = "coucou";
    std::string string3 = "encore";

    size_t index1 = set.insert(string1);
    size_t index2 = set.insert(string2);
    size_t index3 = set.insert(string3);

    bool success1 = string1 == *set.at(index1);
    bool success2 = string2 == *set.at(index2);
    bool success3 = string3 == *set.at(index3);

    set.delet(index2);

    bool success4 = string1 == *set.at(index1);
    bool success5 = string3 == *set.at(index3);

    std::string string4 = "super";

    size_t index4 = set.insert(string4);

    bool success6 = string1 == *set.at(index1);
    bool success7 = string4 == *set.at(index4);
    bool success8 = string3 == *set.at(index3);

    bool success9 = string1 != *set.at(index1);

    return success1 && success2 && success3 && success4 && success5 && success6 && success7 && success8 && success9;
}
