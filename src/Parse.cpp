#include "Parse.hpp"
#include "Custom_Error.hpp"
#include <filesystem>

const std::vector<std::vector<int>>& ParseFile::getParsedContent() const {
	return this->_parsedContent;
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

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

void ParseFile::parseContent() {
    auto& content = this->_content;
    while (!content.empty()) {
        const auto& ligne = content.front();
        std::vector<std::string> tokens = split(ligne, ' ');

        std::vector<int> ligneNumerique;
        for (const auto& token : tokens) {
            if (!token.empty() && token[0] == '#')
                break;
            else if (std::all_of(token.begin(), token.end(), ::isdigit))
                ligneNumerique.push_back(std::stoi(token));
            else if (!token.empty())
                throw CustomError("Erreur de format: token inattendu '" + token + "' dans le fichier.");
        }

        if (!ligneNumerique.empty()) {
            _parsedContent.push_back(ligneNumerique);
        }
        content.pop();
    }
}

void ParseFile::verifierSequenceComplete() const {
    std::set<int> nombresVus;

    for (const auto& ligne : _parsedContent) {
        for (int num : ligne) {
            nombresVus.insert(num);
        }
    }

    size_t tailleAttendue = _parsedContent.size();
    size_t nombreTotalAttendu = tailleAttendue * tailleAttendue;

    if (nombresVus.size() != nombreTotalAttendu) {
        throw CustomError("Erreur: le puzzle ne contient pas le bon nombre de valeurs uniques.");
    }

    for (size_t i = 0; i < nombreTotalAttendu; ++i) {
        if (nombresVus.find(static_cast<int>(i)) == nombresVus.end()) {
            throw CustomError("Erreur: nombre " + std::to_string(i) + " manquant dans le puzzle.");
        }
    }
}

void ParseFile::verifierTaille() {
    if (_parsedContent.empty()) {
        throw CustomError("Erreur: Le fichier est vide ou ne contient pas de données valides.");
    }

    size_t tailleAttendue = static_cast<size_t>(_parsedContent[0][0]);
    if (_parsedContent.size() - 1 != tailleAttendue) {
        throw CustomError("Erreur: Le nombre de lignes ne correspond pas à la taille attendue.");
    }

    for (size_t i = 1; i < _parsedContent.size(); ++i) {
        if (_parsedContent[i].size() != tailleAttendue) {
            throw CustomError("Erreur: Mauvais nombre de chiffres à la ligne " + std::to_string(i) + ".");
        }
    }
	_parsedContent.erase(_parsedContent.begin());
}

void ParseFile::afficherParsedContent() {
    for (const auto& ligne : _parsedContent) {
        for (int num : ligne) {
            std::cout << num << ' ';
        }
        std::cout << std::endl;
    }
}

ParseFile::ParseFile(const std::string& fileName) {
	this->readInputFile(fileName);
	this->parseContent();
	this->verifierTaille();
	this->verifierSequenceComplete();
	std::cout << "Valid content afther parsing :" << std::endl;
	this->afficherParsedContent();
}
