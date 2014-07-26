#ifndef TESTRUNNER_H
#define TESTRUNNER_H


class TestRunnerServer
{
    public:
        TestRunner();
        ~TestRunner();
        void run();
        void testPeers();
        void testChatrooms();
        void testServer();
};

#endif // TESTRUNNER_H
