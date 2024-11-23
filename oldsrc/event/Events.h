#pragma once

namespace Events {
    class Cancellable {
    public:
        void cancel() {
            this->cancelled = true;
        }

        bool isCancelled() {
            return this->cancelled;
        }
    private:
        bool cancelled = false;
    };

    class TestEvent : public Cancellable {
    public:
        explicit TestEvent(int _test) : test(_test) {};

        int getTest() {
            return this->test;
        }
    private:
        int test;
    };
};