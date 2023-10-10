#pragma once

#include <string>
#include <mutex>

namespace Symbios
{
    namespace Global
    {
        class Assets
        {
        private:
            static Assets *pinstance_;
            static std::mutex mutex_;

        protected:
            Assets() {}
            ~Assets() {}

        public:
            Assets(Assets &other) = delete;
            void operator=(const Assets &) = delete;

            static Assets *GetInstance();
        };
    }
}
