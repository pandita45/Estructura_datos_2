#include "pugixml.hpp"
#include "tree.hpp"
#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char **argv) {
  if (argc == 1) {
    std::cout << "XD\n";
    exit(-1);
  }
  char *ruta = argv[1];
  Arbol arbol;
  Nodo raiz = Nodo("root");
  arbol.raiz = &raiz;

  for (const auto &archivo : std::filesystem::directory_iterator(ruta)) {
    if (!archivo.is_regular_file() || archivo.path().extension() != ".xml") {
      continue;
    }
    pugi::xml_document doc;
    pugi::xml_parse_result resultado = doc.load_file(archivo.path().c_str());
    if (!resultado) {
      continue;
    }
    pugi::xml_node xml = doc.select_node("//GoodreadsResponse/book").node();
    auto libro = raiz.agregarHijo("Libro");
    auto id = libro->agregarHijo("id");
    id->agregarHijo(xml.child_value("id"));

    auto titulo = libro->agregarHijo("titulo");
    titulo->agregarHijo(xml.child_value("title"));

    auto isbn = libro->agregarHijo("isbn");
    isbn->agregarHijo(xml.child_value("isbn"));

    auto year = libro->agregarHijo("year");
    year->agregarHijo(xml.child_value("publication_year"));

    auto idioma = libro->agregarHijo("idioma");
    idioma->agregarHijo(xml.child_value("language_code"));

    auto descripcion = libro->agregarHijo("descripcion");
    descripcion->agregarHijo(xml.child_value("description"));

    auto rating = libro->agregarHijo("rating");
    rating->agregarHijo(xml.child_value("average_rating"));

    auto paginas = libro->agregarHijo("paginas");
    paginas->agregarHijo(xml.child_value("num_pages"));

    auto similares = libro->agregarHijo("similares");
    auto conjuntoSimilares = doc.select_nodes("//similar_books/book");

    for (auto similar : conjuntoSimilares) {
      xml = similar.node();
      libro = similares->agregarHijo("Libro");

      auto titulo = libro->agregarHijo("titulo");
      titulo->agregarHijo(xml.child_value("title"));

      auto isbn = libro->agregarHijo("isbn");
      isbn->agregarHijo(xml.child_value("isbn"));

      auto year = libro->agregarHijo("year");
      year->agregarHijo(xml.child_value("publication_year"));
    }
  }
  for(auto n : arbol.listarPrecursores()){
    std::cout << n << '\n';
  }
}
