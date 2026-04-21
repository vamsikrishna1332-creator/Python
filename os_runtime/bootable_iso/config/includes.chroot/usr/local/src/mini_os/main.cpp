#include <filesystem>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;

enum class AppKind {
    Exe,
    Apk,
    FlatpakRef,
    Unknown,
};

struct LaunchRequest {
    AppKind kind;
    std::string target;
    std::vector<std::string> args;
};

std::string shell_escape(const std::string& value) {
    std::ostringstream escaped;
    escaped << '"';
    for (char c : value) {
        if (c == '"' || c == '\\') {
            escaped << '\\';
        }
        escaped << c;
    }
    escaped << '"';
    return escaped.str();
}

AppKind infer_kind(const std::string& target) {
    if (target.ends_with(".exe")) {
        return AppKind::Exe;
    }
    if (target.ends_with(".apk")) {
        return AppKind::Apk;
    }
    if (target.rfind("flatpak:", 0) == 0) {
        return AppKind::FlatpakRef;
    }
    return AppKind::Unknown;
}

std::string usage() {
    return
        "Usage:\n"
        "  mini_os run <path-to.exe> [args...]\n"
        "  mini_os run <path-to.apk> [args...]\n"
        "  mini_os run flatpak:<app-id> [args...]\n\n"
        "Examples:\n"
        "  mini_os run game.exe --fullscreen\n"
        "  mini_os run mobile.apk\n"
        "  mini_os run flatpak:org.mozilla.firefox --private-window\n";
}

std::optional<LaunchRequest> parse_args(int argc, char** argv) {
    if (argc < 3) {
        return std::nullopt;
    }

    const std::string command = argv[1];
    if (command != "run") {
        return std::nullopt;
    }

    LaunchRequest request;
    request.target = argv[2];
    request.kind = infer_kind(request.target);

    for (int i = 3; i < argc; ++i) {
        request.args.emplace_back(argv[i]);
    }

    return request;
}

std::string to_shell_cmd(const LaunchRequest& request) {
    std::ostringstream cmd;

    switch (request.kind) {
        case AppKind::Exe:
            cmd << "wine " << shell_escape(request.target);
            break;
        case AppKind::Apk:
            cmd << "waydroid app install " << shell_escape(request.target);
            break;
        case AppKind::FlatpakRef: {
            auto app_id = request.target.substr(std::string("flatpak:").size());
            cmd << "flatpak run " << shell_escape(app_id);
            break;
        }
        case AppKind::Unknown:
            throw std::invalid_argument(
                "Unknown app type. Use .exe, .apk, or flatpak:<app-id>.");
    }

    for (const auto& arg : request.args) {
        cmd << ' ' << shell_escape(arg);
    }

    return cmd.str();
}

int main(int argc, char** argv) {
    try {
        auto parsed = parse_args(argc, argv);
        if (!parsed.has_value()) {
            std::cerr << usage();
            return 1;
        }

        std::string command = to_shell_cmd(parsed.value());
        std::cout << "[mini_os] Launch pipeline: " << command << "\n";
        int rc = std::system(command.c_str());

        if (rc != 0) {
            std::cerr << "[mini_os] runtime command failed with code " << rc
                      << ". Ensure wine/waydroid/flatpak are installed.\n";
            return 2;
        }

        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n\n";
        std::cerr << usage();
        return 1;
    }
}
