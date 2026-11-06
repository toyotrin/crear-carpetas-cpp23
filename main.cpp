#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string folderPath;

    if (argc > 1) {
        folderPath = argv[1];
    } else {
        std::cout << "Ruta de la carpeta o subcarpetas que quieres crear: ";
        std::getline(std::cin, folderPath);
    }

    if (folderPath.empty()) {
        std::cerr << "Error: la ruta no puede estar vacía.\n";
        return 1;
    }

    std::error_code error;
    const bool created = std::filesystem::create_directories(folderPath, error);

    if (error) {
        std::cerr << "No se pudo crear la carpeta: " << error.message() << '\n';
        return 1;
    }

    if (created) {
        std::cout << "Carpeta(s) creada(s): " << folderPath << '\n';
        return 0;
    }

    if (std::filesystem::is_directory(folderPath, error) && !error) {
        std::cout << "La carpeta ya existe: " << folderPath << '\n';
        return 0;
    }

    std::cerr << "No se pudo crear la ruta indicada.\n";
    return 1;
}
