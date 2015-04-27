void function() {
    int a;
}

void function(int param1, void *param2) {

}

void no_decl_function(int param1, void *param2);

void func2() {
    class inner {
    };

    inner a;
    {
        auto b = a;
    }
}