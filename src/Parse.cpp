#include "Parse.hpp"
#include "Custom_Error.hpp"
#include <filesystem>


void ParseFile::setContent(std::queue<std::string>&& content) {
	this->_content = std::move(content);
}

const std::vector<std::string>& ParseFile::getParsedContent() const {
	return this->_parsedContent;
}

const std::queue<std::string>& ParseFile::getContent() const {
	return this->_content;
}

void ParseFile::readInputFile(const std::string& cheminDuFichier) {
	std::filesystem::path filePath(cheminDuFichier);

	// Vérifier si le chemin existe et est un fichier
	if (!std::filesystem::exists(filePath) || !std::filesystem::is_regular_file(filePath)) {
		throw CustomError("Error: Le chemin spécifié n'est pas un fichier: " + cheminDuFichier);
	}

	std::string ligne;
	std::ifstream fichier(cheminDuFichier);

	if (!fichier.is_open()) {
		throw CustomError("Error: Impossible d'ouvrir le fichier : " + cheminDuFichier);
	}

	while (std::getline(fichier, ligne)) {
		this->_content.push(std::move(ligne));
	}

	fichier.close();
}

ParseFile::ParseFile(const std::string& fileName) {
	this->readInputFile(fileName);

	auto content = this->getContent();

	while (!content.empty()) {
		std::cout << content.front() << std::endl;
		content.pop();
	}
}
