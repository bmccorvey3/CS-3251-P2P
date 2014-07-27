#ifndef TESTRUNNER_H
#define TESTRUNNER_H


class TestRunnerServer
{
    public:
		TestRunnerServer();
        ~TestRunnerServer();
        void run();
        void testPeers();
        void testChatrooms();
        void testServer();
};

#endif // TESTRUNNER_H
