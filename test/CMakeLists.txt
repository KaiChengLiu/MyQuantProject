add_executable(MyQuantTests
    test_main.cpp
    test_api.cpp
)

target_link_libraries(MyQuantTests
    PRIVATE
        GTest::gtest
        GTest::gtest_main
        CURL::libcurl
        nlohmann_json::nlohmann_json
)

add_test(NAME MyQuantTests COMMAND MyQuantTests)
