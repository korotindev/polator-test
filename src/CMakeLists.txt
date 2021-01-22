file(GLOB sources *.cpp *.hpp)
add_executable(network-sdk ${sources})
target_compile_options(network-sdk PUBLIC 
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -Wcast-align
    -Wcast-qual
    -Wctor-dtor-privacy
    -Wenum-compare
    -Wfloat-equal
    -Wnon-virtual-dtor
    -Wold-style-cast
    -Woverloaded-virtual
    -Wredundant-decls
    -Wsign-promo
    -Wno-error=unused-function
)