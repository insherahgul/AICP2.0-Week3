#include <iostream>
#include <string>
#include <limits>

using namespace std;

// Function to clear input buffer
void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Function to record milk yield for a cow for a specific day and shift
void recordYield(const string& cowId, int day, const string& shift, double yields[][2][100], int cowIndex, double weeklyYields[]) {
    double yield;
    cout << "Enter the milk yield for cow " << cowId << " on day " << day << " " << shift << " shift: ";
    cin >> yield;

    while (cin.fail() || yield < 0) {
        cout << "Invalid input. Please enter a non-negative number." << endl;
        cout << "Enter the milk yield for cow " << cowId << " on day " << day << " " << shift << " shift: ";
        clearInputBuffer();
        cin >> yield;
    }

    // Update weekly yield
    weeklyYields[cowIndex] += yield;

    // 0 for morning, 1 for evening
    yields[day - 1][(shift == "morning") ? 0 : 1][cowIndex] = yield;
}

// Function to display recorded milk yields for morning and evening shifts for a specific day
void displayDayYields(int numCows, const string cowIds[], double yields[][2][100], int day) {
    cout << "\nRecorded milk yields for day " << day << ":" << endl;
    cout << "Morning shift:" << endl;
    for (int cow = 0; cow < numCows; ++cow) {
        cout << "Cow " << cowIds[cow] << ": " << yields[day - 1][0][cow] << endl;
    }
    cout << "Evening shift:" << endl;
    for (int cow = 0; cow < numCows; ++cow) {
        cout << "Cow " << cowIds[cow] << ": " << yields[day - 1][1][cow] << endl;
    }
}

int main() {
    int numCows;
    cout << "Enter the number of cows in the herd for the week: ";
    cin >> numCows;
    clearInputBuffer(); // Clear input buffer after reading integer

    // Ensure numCows is within valid range
    if (numCows <= 0 || numCows > 100) {
        cout << "Invalid number of cows. Please enter a number between 1 and 100." << endl;
        return 1;
    }

    string cowIds[100]; // Array to store cow IDs
    double yields[7][2][100] = {}; // 7 days, 2 shifts (morning, evening), up to 100 cows
    double weeklyYields[100] = {}; // Weekly yield for each cow

    // Enter 3-digit IDs for each cow
    for (int cow = 0; cow < numCows; ++cow) {
        cout << "Enter the 3-digit identity code of cow " << cow + 1 << ": ";
        cin >> cowIds[cow];
        clearInputBuffer(); // Clear input buffer after reading cow ID

        // Ensure cow ID is a 3-digit number
        while (cowIds[cow].length() != 3 || cowIds[cow].find_first_not_of("0123456789") != string::npos) {
            cout << "Invalid input. Identity code must be a 3-digit number." << endl;
            cout << "Enter the 3-digit identity code of cow " << cow + 1 << ": ";
            cin >> cowIds[cow];
            clearInputBuffer(); // Clear input buffer after reading cow ID
        }
    }

    int day;
    string shiftChoice;

    // Variables for total weekly volume and total yield per cow
    double totalWeeklyVolume = 0.0;
    double totalYieldPerCow = 0.0;
    double maxWeeklyYield = 0.0;
    string mostProductiveCowId;
    bool lowYieldCows[100] = {}; // Array to mark cows with low yields for four days or more

    while (true) { // Loop indefinitely until manually terminated
        // Get day choice
        cout << "\nSelect day (1-7) or enter 0 to show recorded yields and terminate the program: ";
        cin >> day;
        if (cin.fail() || (day < 0 || day > 7)) {
            cout << "Invalid input. Please enter a number between 0 and 7." << endl;
            clearInputBuffer();
            continue;
        }

        if (day == 0) {
            // Show recorded yields and terminate the program
            for (int d = 1; d <= 7; ++d) {
                displayDayYields(numCows, cowIds, yields, d);
            }

            // Calculate total weekly volume and average yield per cow
            for (int d = 0; d < 7; ++d) {
                for (int shift = 0; shift < 2; ++shift) {
                    for (int cow = 0; cow < numCows; ++cow) {
                        totalWeeklyVolume += yields[d][shift][cow];
                    }
                }
            }
            totalYieldPerCow = totalWeeklyVolume / (numCows * 7); // 7 days in a week

            // Display total weekly volume and average yield per cow
            cout << "\nTotal weekly volume of milk for the herd: " << totalWeeklyVolume << " litres" << endl;
            cout << "Average yield per cow in a week: " << totalYieldPerCow << " litres" << endl;

            // Identify the most productive cow
            for (int cow = 0; cow < numCows; ++cow) {
                if (weeklyYields[cow] > maxWeeklyYield) {
                    maxWeeklyYield = weeklyYields[cow];
                    mostProductiveCowId = cowIds[cow];
                }
            }
            cout << "The most productive cow is: " << mostProductiveCowId << " with a weekly yield of " << maxWeeklyYield << " litres." << endl;

            // Identify cows with low yields for four days or more
            for (int cow = 0; cow < numCows; ++cow) {
                int count = 0;
                for (int d = 0; d < 7; ++d) {
                    if (yields[d][0][cow] + yields[d][1][cow] < 12) {
                        count++;
                    }
                }
                if (count >= 4) {
                    cout << "Cow " << cowIds[cow] << " has a low yield for four days or more." << endl;
                }
            }

            return 0; // Terminate the program
        }

        // Record milk yield for morning shift
        cout << "Recording milk yield for morning shift..." << endl;
        for (int cow = 0; cow < numCows; ++cow) {
            recordYield(cowIds[cow], day, "morning", yields, cow, weeklyYields);
        }

        // Record milk yield for evening shift
        cout << "Recording milk yield for evening shift..." << endl;
        for (int cow = 0; cow < numCows; ++cow) {
            recordYield(cowIds[cow], day, "evening", yields, cow, weeklyYields);
        }
    }

    return 0;
}

