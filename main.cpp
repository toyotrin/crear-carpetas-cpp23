#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

namespace fs = std::filesystem;
using ProjectFile = std::pair<std::string, std::string>;

std::vector<ProjectFile> makeTemplate(int option) {
    switch (option) {
    case 1: // Aplicación de consola
        return {
            {"Makefile", R"FILE(CXX      = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -Wpedantic -Iinclude
TARGET   = build/app
SOURCES  = src/main.cpp src/mensaje.cpp

.PHONY: all run clean
all: $(TARGET)

$(TARGET): $(SOURCES) include/mensaje.hpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -rf build
)FILE"},
            {"include/mensaje.hpp", R"FILE(#pragma once

void mostrarMensaje();
)FILE"},
            {"src/main.cpp", R"FILE(#include "mensaje.hpp"

int main() {
    mostrarMensaje();
    return 0;
}
)FILE"},
            {"src/mensaje.cpp", R"FILE(#include "mensaje.hpp"

#include <iostream>

void mostrarMensaje() {
    std::cout << "¡Hola desde tu nuevo proyecto C++23!\\n";
}
)FILE"},
            {"README.md", R"FILE(# Aplicación de consola C++23

## Compilar

```sh
make
```

## Ejecutar

```sh
make run
```

## Limpiar

```sh
make clean
```
)FILE"}
        };
    case 2: // Proyecto modular
        return {
            {"Makefile", R"FILE(CXX      = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -Wpedantic -Iinclude
TARGET   = build/app
SOURCES  = src/main.cpp src/Calculadora.cpp

.PHONY: all run clean
all: $(TARGET)

$(TARGET): $(SOURCES) include/Calculadora.hpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -rf build
)FILE"},
            {"include/Calculadora.hpp", R"FILE(#pragma once

class Calculadora {
public:
    int sumar(int a, int b) const;
};
)FILE"},
            {"src/Calculadora.cpp", R"FILE(#include "Calculadora.hpp"

int Calculadora::sumar(int a, int b) const {
    return a + b;
}
)FILE"},
            {"src/main.cpp", R"FILE(#include "Calculadora.hpp"

#include <iostream>

int main() {
    const Calculadora calculadora;
    std::cout << "2 + 3 = " << calculadora.sumar(2, 3) << '\\n';
    return 0;
}
)FILE"},
            {"README.md", R"FILE(# Proyecto modular C++23

El código está separado entre declaraciones (`include/`) e implementaciones (`src/`).

```sh
make       # compilar
make run   # compilar y ejecutar
make clean # borrar el ejecutable generado
```
)FILE"}
        };
    case 3: // Biblioteca
        return {
            {"Makefile", R"FILE(CXX      = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -Wpedantic -Iinclude
AR       = ar
TARGET   = build/libsaludos.a
OBJECTS  = build/saludos.o

.PHONY: all clean
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) rcs $(TARGET) $(OBJECTS)

$(OBJECTS): src/saludos.cpp include/saludos.hpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c src/saludos.cpp -o $(OBJECTS)

clean:
	rm -rf build
)FILE"},
            {"include/saludos.hpp", R"FILE(#pragma once

#include <string>

std::string crearSaludo(const std::string& nombre);
)FILE"},
            {"src/saludos.cpp", R"FILE(#include "saludos.hpp"

std::string crearSaludo(const std::string& nombre) {
    return "Hola, " + nombre + "!";
}
)FILE"},
            {"examples/main.cpp", R"FILE(#include "saludos.hpp"

#include <iostream>

int main() {
    std::cout << crearSaludo("mundo") << '\\n';
    return 0;
}
)FILE"},
            {"README.md", R"FILE(# Biblioteca C++23

`make` crea `build/libsaludos.a`. El ejemplo puede compilarse desde la raíz con:

```sh
g++ -std=c++23 -Iinclude examples/main.cpp src/saludos.cpp -o build/ejemplo
./build/ejemplo
```

Usa `make clean` para borrar los artefactos generados.
)FILE"}
        };
    case 4: // Juego de consola
        return {
            {"Makefile", R"FILE(CXX      = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -Wpedantic -Iinclude
TARGET   = build/juego
SOURCES  = src/main.cpp src/Juego.cpp

.PHONY: all run clean
all: $(TARGET)

$(TARGET): $(SOURCES) include/Juego.hpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -rf build
)FILE"},
            {"include/Juego.hpp", R"FILE(#pragma once

class Juego {
public:
    void ejecutar() const;
};
)FILE"},
            {"src/Juego.cpp", R"FILE(#include "Juego.hpp"

#include <iostream>

void Juego::ejecutar() const {
    std::cout << "Adivina el número (del 1 al 5): ";
    int numero = 0;
    if (!(std::cin >> numero)) {
        std::cout << "Entrada no válida.\\n";
        return;
    }

    if (numero == 3) {
        std::cout << "¡Correcto!\\n";
    } else {
        std::cout << "No era ese; el número era 3.\\n";
    }
}
)FILE"},
            {"src/main.cpp", R"FILE(#include "Juego.hpp"

int main() {
    Juego juego;
    juego.ejecutar();
    return 0;
}
)FILE"},
            {"README.md", R"FILE(# Juego de consola C++23

```sh
make       # compilar
make run   # jugar
make clean # limpiar
```
)FILE"}
        };
    default:
        return {};
    }
}

int main(int argc, char* argv[]) {
    std::string projectPath;
    if (argc > 1) {
        projectPath = argv[1];
    } else {
        std::cout << "Nombre o ruta de la carpeta del proyecto: ";
        std::getline(std::cin, projectPath);
    }

    if (projectPath.empty()) {
        std::cerr << "Error: el nombre o la ruta no puede estar vacía.\n";
        return 1;
    }

    std::cout << "\n¿Qué proyecto deseas crear?\n"
              << "  1. Aplicación de consola\n"
              << "  2. Proyecto modular (clase Calculadora)\n"
              << "  3. Biblioteca C++\n"
              << "  4. Juego de consola (adivina el número)\n"
              << "Opción: ";

    int option = 0;
    if (!(std::cin >> option)) {
        std::cerr << "Error: introduce un número de opción válido.\n";
        return 1;
    }

    const std::vector<ProjectFile> files = makeTemplate(option);
    if (files.empty()) {
        std::cerr << "Opción no válida.\n";
        return 1;
    }

    std::error_code error;
    const fs::path root(projectPath);
    if (fs::exists(root, error) && !error && !fs::is_directory(root, error)) {
        std::cerr << "Error: la ruta existe y no es una carpeta.\n";
        return 1;
    }

    // No sobrescribir ningún archivo preexistente.
    for (const auto& [relativePath, contents] : files) {
        (void)contents;
        const fs::path destination = root / relativePath;
        if (fs::exists(destination, error)) {
            std::cerr << "Error: ya existe " << destination.string()
                      << ". No se ha sobrescrito ningún archivo.\n";
            return 1;
        }
        if (error) {
            std::cerr << "Error al comprobar la ruta: " << error.message() << '\n';
            return 1;
        }
    }

    fs::create_directories(root, error);
    if (error) {
        std::cerr << "No se pudo crear la carpeta del proyecto: "
                  << error.message() << '\n';
        return 1;
    }

    for (const auto& [relativePath, contents] : files) {
        const fs::path destination = root / relativePath;
        fs::create_directories(destination.parent_path(), error);
        if (error) {
            std::cerr << "No se pudo crear una subcarpeta: " << error.message() << '\n';
            return 1;
        }

        std::ofstream output(destination, std::ios::binary);
        if (!output) {
            std::cerr << "No se pudo crear el archivo: " << destination.string() << '\n';
            return 1;
        }
        output << contents;
        if (!output) {
            std::cerr << "Error al escribir el archivo: " << destination.string() << '\n';
            return 1;
        }
    }

    std::cout << "\n¡Proyecto creado en " << root.string() << "!\n"
              << "Archivos generados: " << files.size() << "\n"
              << "Entra en la carpeta y ejecuta: make\n";
    return 0;
}
