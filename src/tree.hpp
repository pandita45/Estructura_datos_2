#include <iostream>
#include <string>
#include <vector>

class Nodo {
public:
  std::string etiqueta;
  std::vector<Nodo *> hijos;

  Nodo(std::string etiqueta) : etiqueta(etiqueta) {}
  ~Nodo() {
    for (auto hijo : hijos)
      delete hijo;
  }
  std::vector<Nodo *> getHijos() { return hijos; };
  std::string getEtiqueta() { return etiqueta; };
  Nodo *agregarHijo(std::string etiqueta) {
    Nodo *hijo = new Nodo(etiqueta);
    hijos.push_back(hijo);
    return hijo;
  };
  void listar() {
    if (etiqueta == "id" && !hijos.empty())
      std::cout << hijos.front()->getEtiqueta() << '\n';

    for (Nodo *hijo : hijos) {
      (*hijo).listar();
    }
  }
};

class Arbol {

private:
  bool precursores(Nodo *libro_actual) {
    if (libro_actual == nullptr)
      return false;

    int year = 0;
    std::vector<Nodo *> libros_similares;
    for (Nodo *atributo : libro_actual->getHijos()) {
      // lo mismo de hace 2 ifs
      if (atributo->getEtiqueta() != "similares") {
        continue;
      }
      libros_similares = atributo->getHijos();
      break;
    }

    for (Nodo *atributo : libro_actual->getHijos()) {
      if (atributo->getEtiqueta() != "year" || atributo->getHijos().empty() ||
          atributo->getHijos().front()->getEtiqueta() == "") {
        continue;
      }
      year = std::stoi(atributo->getHijos().front()->getEtiqueta());
      break;
    }

    for (Nodo *libro : libros_similares) {
      for (Nodo *atributo : libro->getHijos()) {
        if (atributo->getEtiqueta() != "year") {
          continue;
        }

        if (atributo->getHijos().empty() ||
            atributo->getHijos().front()->getEtiqueta() == "" ||
            std::stoi(atributo->getHijos().front()->getEtiqueta()) <= year)
          return false;
      }
    }
    return true;
  }

public:
  Nodo *raiz;
  void listar() { raiz->listar(); }
  void borrarRatings(double ratingMinimo) {
    std::vector<Nodo *> nuevosHijos;
    for (Nodo *libro : raiz->getHijos()) {
      for (Nodo *atributo : libro->getHijos()) {
        // me da flojera tener muchos if anidados, si no es el atributo rating
        // o si no tiene rating, lo ignoro
        if (atributo->getEtiqueta() != "rating" || atributo->getHijos().empty())
          continue;

        double rating = std::stod(atributo->getHijos().front()->getEtiqueta());
        if (rating > ratingMinimo) {
          nuevosHijos.push_back(libro);
        }
        break;
      }
      if (nuevosHijos.empty() || nuevosHijos.back() != libro) {
        delete libro;
      }
    }
    raiz->hijos = nuevosHijos;
  }
  Nodo *buscarId(long id) {
    for (Nodo *libro : raiz->getHijos()) {
      long id_actual;
      for (Nodo *atributo : libro->getHijos()) {
        // lo mismo de hace 2 ifs
        if (atributo->getEtiqueta() != "id" || atributo->getHijos().empty()) {
          continue;
        }
        id_actual = std::stol(atributo->getHijos().front()->getEtiqueta());
        break;
      }
      if (id_actual != id) {
        continue;
      }
      return libro;
    }
    return nullptr;
  }
  bool precursores(long id) { return precursores(buscarId(id)); }
  std::vector<std::string> listarPrecursores() {
    std::vector<std::string> listaPrecursores;
    for (Nodo *nodo : raiz->getHijos()) {
      if (!precursores(nodo)) {
        continue;
      }
      bool tieneYear = false;
      for (Nodo *atributo : nodo->getHijos()) {
        if (atributo->getEtiqueta() == "year" &&
            atributo->getHijos().front()->getEtiqueta() != "") {
          tieneYear = true;
          break;
        }
      }
      if (!tieneYear)
        continue;
      for (Nodo *atributo : nodo->getHijos()) {
        if (atributo->getEtiqueta() == "id") {
          listaPrecursores.push_back(
              atributo->getHijos().front()->getEtiqueta());
          break;
        }
      }
    }
    return listaPrecursores;
  };
};
