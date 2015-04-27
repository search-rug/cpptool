class global {
};
namespace n {
    void external_func() {
        class floating_func {
        };
    }

    enum normal_enum {};

    enum class class_enum{};

    struct str_test {};

    namespace m {
        class in_namespace {
            class in_class {
            };

            void main() {
                class in_function {
                };

                {
                    class in_inner_scope {
                    };
                }
            }
        };
    }
}