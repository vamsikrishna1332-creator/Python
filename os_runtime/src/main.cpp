#include <cstdlib>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

enum class Action {
    Run,
    Install,
};

enum class AppKind {
    Exe,
    Msi,
    ApkFile,
    ApkPackage,
    FlatpakRef,
    Unknown,
};

struct LaunchRequest {
    Action action;
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
    if (target.ends_with(".msi")) {
        return AppKind::Msi;
    }
    if (target.ends_with(".apk")) {
        return AppKind::ApkFile;
    }
    if (target.rfind("apk:", 0) == 0) {
        return AppKind::ApkPackage;
    }
    if (target.rfind("flatpak:", 0) == 0) {
        return AppKind::FlatpakRef;
    }
    return AppKind::Unknown;
}

std::string usage() {
    return
        "Usage:\n"
        "  mini_os install <setup.exe|setup.msi|app.apk|flatpak:app-id>\n"
        "  mini_os run <app.exe|flatpak:app-id|apk:package.name> [args...]\n\n"
        "Examples:\n"
        "  mini_os install setup.exe\n"
        "  mini_os install game.msi\n"
        "  mini_os install mobile.apk\n"
        "  mini_os run flatpak:org.mozilla.firefox --private-window\n"
        "  mini_os run apk:com.android.chrome\n";
}

std::optional<LaunchRequest> parse_args(int argc, char** argv) {
    if (argc < 3) {
        return std::nullopt;
    }

    LaunchRequest request;
    const std::string action = argv[1];
    if (action == "run") {
        request.action = Action::Run;
    } else if (action == "install") {
        request.action = Action::Install;
    } else {
        return std::nullopt;
    }

    request.target = argv[2];
    request.kind = infer_kind(request.target);

    for (int i = 3; i < argc; ++i) {
        request.args.emplace_back(argv[i]);
    }

    return request;
}

std::string build_install_cmd(const LaunchRequest& request) {
    std::ostringstream cmd;

    switch (request.kind) {
        case AppKind::Exe:
            cmd << "wine " << shell_escape(request.target);
            break;
        case AppKind::Msi:
            cmd << "wine msiexec /i " << shell_escape(request.target);
            break;
        case AppKind::ApkFile:
            cmd << "waydroid app install " << shell_escape(request.target);
            break;
        case AppKind::FlatpakRef: {
            auto app_id = request.target.substr(std::string("flatpak:").size());
            cmd << "flatpak install -y flathub " << shell_escape(app_id);
            break;
        }
        case AppKind::ApkPackage:
            throw std::invalid_argument(
                "For install use an APK file path, not apk:<package>. Example: mini_os install app.apk");
        case AppKind::Unknown:
            throw std::invalid_argument(
                "Unknown install target. Use .exe, .msi, .apk, or flatpak:<app-id>.");
    }

    return cmd.str();
}

std::string build_run_cmd(const LaunchRequest& request) {
    std::ostringstream cmd;

    switch (request.kind) {
        case AppKind::Exe:
        case AppKind::Msi:
            cmd << "wine " << shell_escape(request.target);
            break;
        case AppKind::ApkPackage: {
            auto package_name = request.target.substr(std::string("apk:").size());
            cmd << "waydroid app launch " << shell_escape(package_name);
            break;
        }
        case AppKind::FlatpakRef: {
            auto app_id = request.target.substr(std::string("flatpak:").size());
            cmd << "flatpak run " << shell_escape(app_id);
            break;
        }
        case AppKind::ApkFile:
            throw std::invalid_argument(
                "Running APK files directly is not supported. Install first: mini_os install app.apk, then run apk:<package>.");
        case AppKind::Unknown:
            throw std::invalid_argument(
                "Unknown run target. Use .exe, flatpak:<app-id>, or apk:<package>.");
    }

    for (const auto& arg : request.args) {
        cmd << ' ' << shell_escape(arg);
    }

    return cmd.str();
}

std::string to_shell_cmd(const LaunchRequest& request) {
    if (request.action == Action::Install) {
        return build_install_cmd(request);
    }
    return build_run_cmd(request);
}

int main(int argc, char** argv) {
    try {
        auto parsed = parse_args(argc, argv);
        if (!parsed.has_value()) {
            std::cerr << usage();
            return 1;
        }

        std::string command = to_shell_cmd(parsed.value());
        std::cout << "[mini_os] Command: " << command << "\n";
        int rc = std::system(command.c_str());

        if (rc != 0) {
            std::cerr << "[mini_os] command failed with code " << rc
                      << ". Ensure wine/waydroid/flatpak are installed and configured.\n";
            return 2;
        }

        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n\n";
        std::cerr << usage();
        return 1;
    }
}
