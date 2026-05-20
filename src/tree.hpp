#include <iostream>
#include <string>
#include <vector>

class Nodo {
public:
  std::string etiqueta;
  std::vector<Nodo *> hijos;

  //etiqueta es el nombre del nodo, por ejemplo "titulo" o "id"
  Nodo(std::string etiqueta) : etiqueta(etiqueta) {}
  ~Nodo() {
    for (auto hijo : hijos)
      delete hijo;
  }
  // devuelve los hijos del nodo
  std::vector<Nodo *> getHijos() { return hijos; };
  std::string getEtiqueta() { return etiqueta; };
  Nodo *agregarHijo(std::string etiqueta) {
    Nodo *hijo = new Nodo(etiqueta);
    hijos.push_back(hijo);
    return hijo;
  };

  // funcion para listar los libros con recorrido preorder
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
  // revisa si un libro es precursor de los libros similares o sea si su fecha de publicacion es menor a estos
  bool precursores(Nodo *libro_actual) {
    if (libro_actual == nullptr)
      return false;

    int year = 0;
    std::vector<Nodo *> libros_similares;
    for (Nodo *atributo : libro_actual->getHijos()) {
    
      if (atributo->getEtiqueta() != "similares") {
        continue;
      }
      //guarda los libros similares
      libros_similares = atributo->getHijos();
      break;
    }

    for (Nodo *atributo : libro_actual->getHijos()) {
      //verifica que el libro tenga año de publicacion
      //si no tiene año de publicacion o si este es vacio, entonces no es precursor
      if (atributo->getEtiqueta() != "year" ||
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
  
        if (
          //si el libro similar no tiene fecha de publicacion o si esta es menor o igual al del libro actual, entonces no es precursor
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
        //si no hay atributo rating o si este no tiene uno se ignora el libro
        if (atributo->getEtiqueta() != "rating" || atributo->getHijos().empty())
          continue;

        //elimina el libro si su rating es menor o igual al rating minimo
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
  // busca un libro por su id y devuelve un puntero a este, si no lo encuentra devuelve nullptr
  Nodo *buscarId(long id) {
    for (Nodo *libro : raiz->getHijos()) {
      long id_actual;
      for (Nodo *atributo : libro->getHijos()) {
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

  //devuelve una lista con los id de los libros precursores
  std::vector<std::string> listarPrecursores() {
    std::vector<std::string> listaPrecursores;

    for (Nodo *nodo : raiz->getHijos()) {
      //si el libro no es precursor se ignora
      if (!precursores(nodo)) {
        continue;
      }
      bool tieneYear = false;
  
      //verifica que el libro tenga año de publicacion, si no lo tiene se ignora
      for (Nodo *atributo : nodo->getHijos()) {
        if (atributo->getEtiqueta() == "year" &&
            atributo->getHijos().front()->getEtiqueta() != "") {
          tieneYear = true;
          break;
        }
      }
      if (!tieneYear)
        continue;

      //
      for (Nodo *atributo : nodo->getHijos()) {
        if (atributo->getEtiqueta() == "id") {
          //se agrega su id a la lista de precursores
          listaPrecursores.push_back(
              atributo->getHijos().front()->getEtiqueta());
          break;
        }
      }
    }
    return listaPrecursores;
  };
};
