#pragma once
#include"global.h"
class CommandWrapper {
    struct Concept {
        virtual void execute(const std::string& args) = 0;
        virtual ~Concept() = default;
    };

    template<typename T>
    struct Model : Concept {
        T command;
        Model(T cmd) : command(std::move(cmd)) {}
        void execute(const std::string& args) override {
            command.execute(args);
        }
    };

    std::unique_ptr<Concept> impl;

public:
    template<typename T>
    CommandWrapper(T cmd) : impl(std::make_unique<Model<T>>(std::move(cmd))) {}

    void execute(const std::string& args) {
        impl->execute(args);
    }
};