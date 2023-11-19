#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <unordered_map>

class User {
public:
    std::string name;
    double currentLocationLatitude;
    double currentLocationLongitude;
    double destinationLatitude;
    double destinationLongitude;
    std::string travelDateTime;
    bool smokes;
    std::string language;
    std::string musicPreference;

    User(const std::string &n = "", double clat = 0.0, double clon = 0.0, double dlat = 0.0, double dlon = 0.0,
         const std::string &tdt = "", bool s = false, const std::string &lang = "", const std::string &music = "")
        : name(n), currentLocationLatitude(clat), currentLocationLongitude(clon),
          destinationLatitude(dlat), destinationLongitude(dlon), travelDateTime(tdt),
          smokes(s), language(lang), musicPreference(music) {}

   double calculateSimilarity(const User &targetUser) const
        {
            // Calculate the similarity based on user attributes
            double similarity = 0.0;

            // Calculate the distance between current locations and destinations using Haversine formula
            double earthRadius = 6371.0; // Earth's radius in kilometers

            double currentLocationDistance = earthRadius * acos(
                                                               sin(currentLocationLatitude) * sin(targetUser.currentLocationLatitude) +
                                                               cos(currentLocationLatitude) * cos(targetUser.currentLocationLatitude) *
                                                                   cos(currentLocationLongitude - targetUser.currentLocationLongitude));

            double destinationDistance = earthRadius * acos(
                                                           sin(destinationLatitude) * sin(targetUser.destinationLatitude) +
                                                           cos(destinationLatitude) * cos(targetUser.destinationLatitude) *
                                                               cos(destinationLongitude - targetUser.destinationLongitude));

            // Normalize the distances to similarity scores (closer is better)
            double currentLocationSimilarity = 1.0 / (1.0 + currentLocationDistance);
            double destinationSimilarity = 1.0 / (1.0 + destinationDistance);

            // Similarity for other attributes
            double dateSimilarity = (travelDateTime == targetUser.travelDateTime) ? 1.0 : 0.0;
            double smokingSimilarity = (smokes == targetUser.smokes) ? 1.0 : 0.0;
            double languageSimilarity = (language == targetUser.language) ? 1.0 : 0.0;

            // You can customize the music similarity calculation based on your preferences.
            // Here, we assume that if the music preference matches, similarity is 1.0, otherwise, it's 0.0.
            double musicSimilarity = (musicPreference == targetUser.musicPreference) ? 1.0 : 0.0;

            // Calculate the overall similarity
            similarity = (currentLocationSimilarity + destinationSimilarity + dateSimilarity + smokingSimilarity + languageSimilarity + musicSimilarity) / 6.0;

            return similarity;
        }
};

class UserDatabase {
private:
    std::vector<std::vector<User>> usersByDate;

public:
    UserDatabase() {
        usersByDate.resize(365); // Initialize with 365 days (adjust for leap years).
    }

    void addUser(const User &user) {
        int day = getDayFromTravelDateTime(user.travelDateTime);
        if (day >= 0 && day < 365) {
            usersByDate[day].push_back(user);
        }
    }

    std::vector<User> findSimilarUsers(const User &targetUser) const {
        std::vector<User> similarUsers;
        for (const std::vector<User> &usersOnDay : usersByDate) {
            for (const User &user : usersOnDay) {
                double similarity = targetUser.calculateSimilarity(targetUser, user);
                if (similarity >= 0.5) {
                    similarUsers.push_back(user);
                }
            }
        }
        return similarUsers;
    }

    const std::vector<User> &getUsersForDate(const std::string &date) const {
        int day = getDayFromTravelDateTime(date);
        if (day >= 0 && day < 365) {
            return usersByDate[day];
        } else {
            static std::vector<User> empty;
            return empty;
        }
    }

private:
    int getDayFromTravelDateTime(const std::string &date) const {
        std::string dayString = date.substr(0, 2);
        return std::stoi(dayString) - 1;
    }
};

User getUserInput() {
    User user;
    std::cout << "Enter user name: ";
    std::cin.ignore();
    std::getline(std::cin, user.name);

    std::cout << "Enter current location latitude (in radians): ";
    std::cin >> user.currentLocationLatitude;
    std::cout << "Enter current location longitude (in radians): ";
    std::cin >> user.currentLocationLongitude;

    std::cout << "Enter destination latitude (in radians): ";
    std::cin >> user.destinationLatitude;
    std::cout << "Enter destination longitude (in radians): ";
    std::cin >> user.destinationLongitude;

    std::cin.ignore();
    std::cout << "Enter travel date and time (dd mm yyyy): ";
    std::getline(std::cin, user.travelDateTime);
    std::cout << "Do they smoke? (1 for yes, 0 for no): ";
    std::cin >> user.smokes;
    std::cin.ignore();
    std::cout << "Enter spoken language: ";
    std::getline(std::cin, user.language);
    std::cout << "Enter music preference (e.g., Hindi, English, Pop, Rock): ";
    std::getline(std::cin, user.musicPreference);
    return user;
}

void addUserToDatabase(UserDatabase &database) {
    User user = getUserInput();
    database.addUser(user);
    std::cout << "User added to the database.\n";
}

void findAndPrintSimilarUsers(const UserDatabase &database) {
    User targetUser = getUserInput();
    std::vector<User> similarUsers = database.findSimilarUsers(targetUser);

    if (similarUsers.empty()) {
        std::cout << "No similar users found.\n";
    } else {
        std::cout << "Similar users:\n";
        for (const User &user : similarUsers) {
            std::cout << "Name: " << user.name << "\n";
            // Print other user attributes
        }
    }
}

int main() {
    UserDatabase database;
    int choice;

    while (true) {
        std::cout << "Options:\n";
        std::cout << "1. Add User\n";
        std::cout << "2. Find Similar Users\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                addUserToDatabase(database);
                break;
            case 2:
                findAndPrintSimilarUsers(database);
                break;
            case 3:
                std::cout << "Goodbye!\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}