#pragma once
#include <string>
#include <unordered_map>

class AuthManager {
  public:
    AuthManager();

    bool authenticate(const std::string &username,
                      const std::string &password) const;

  private:
    std::unordered_map<std::string, std::string> credentials_;
};
