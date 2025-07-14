#include "json.hpp"
#include "imgui/imgui.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>

using namespace std::pmr;
using json = nlohmann::json;

struct Note {
    std::string title;
    std::string description;
};

std::vector<Note> NOTES;


void to_json(json& j, const Note& note) {
    j = json{{"title", note.title}, {"description", note.description}};
}

void from_json(const json& j, Note& note) {
    j.at("title").get_to(note.title);
    j.at("description").get_to(note.description);
}


void SaveNotes(const std::vector<Note>& notes, const std::string& filename) {
    json j = notes;
    std::ofstream file(filename);
    file << j.dump(4);
}

std::vector<Note> LoadNotes(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return {};
    }

    json j;
    try {
        file >> j;
        return j.get<std::vector<Note>>();
    } catch (...) {
        std::cerr << "Error reading JSON. Starting with empty notes.\n";
        return {};
    }
}



void FillNote(Note &note) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear buffer

    std::cout << "Enter title: ";
    std::getline(std::cin, note.title);
    std::cout << "Enter description: ";
    std::getline(std::cin, note.description);
}

void ShowMenu() {
    std::cout << "====== Options ======\n";
    std::cout << "1 - Add Note\n";
    std::cout << "2 - Show Notes\n";
    std::cout << "3 - Edit Notes\n";
    std::cout << "4 - Delete Notes\n";
    std::cout << "0 - Exit\n";
    std::cout << "Insert Option: ";
}

void AddNote() {
    Note newNote;
    FillNote(newNote);

    NOTES.push_back(newNote);
}

void ShowNotes() {
    for (int i = 0; i < NOTES.size(); i++) {
        std::cout << "Note " << i + 1 << "\n";
        std::cout << "Title: " << NOTES[i].title << "\n";
        std::cout << "Description: " << NOTES[i].description << "\n\n";
    }
}

void EditNote() {
    int n;
    std::cout << "What note would you like to edit: ";
    std::cin >> n;
    FillNote(NOTES[n - 1]);
}

void DeleteNote() {
    int n;
    std::cout << "What note would you like to remove: ";
    std::cin >> n;
    NOTES.erase(NOTES.begin() + (n - 1));
}

int GetOption() {
    int option;
    ShowMenu();
    std::cin >> option;
    return option;
}

int main() {
    NOTES = LoadNotes("notes.json");

    do {
        switch (GetOption()) {
            case 0:
                SaveNotes(NOTES,"notes.json");
                return 0;
            case 1:
                AddNote();
                break;
            case 2:
                ShowNotes();
                break;
            case 3:
                EditNote();
                break;
            case 4:
                DeleteNote();
                break;
            default:
                std::cout << "Invalid Option \n";
        }
    } while (1);
}